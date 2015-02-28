#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define OP_METH_INSTANT 0
#define OP_METH_DIRECT 1
#define OP_METH_DIST 2
#define OP_METH_R_DIRECT 3

#define R_DIR_SOURCE 0
#define R_DIR_DEST 1
#define R_DIR_NONE 2

#define ADDR_R1 0
#define ADDR_R2 1
#define ADDR_R3 2
#define ADDR_R4 3
#define ADDR_R5 4
#define ADDR_R6 5
#define ADDR_R7 6
#define ADDR_R8 7

typedef struct {
	char *param;
	int op_method;
} op_param;

op_param *opp_alloc(void)
{
	return (op_param *)malloc(sizeof(op_param));
}

typedef struct {
	int code;
	int s_operand;
	int d_operand;
} op;

struct oclist
{
	char *code;
	int numOfParams;
	int val;
	struct oclist *next;
};


struct oclist *getOpcodeByCode(char *code);
op_param *get_param(char *line);
char *appendc(char *s, char c);
char *fgetword(FILE *f, char fc);
int get_r_addr(char *name);
int assemble_param(op_param *param, int era, int r_dir_side);
int assemble_param_rdirect(op_param *source_p, op_param *dest_p, int era);
int get_max_dist(int addr_one, int addr_two, int op_addr);
int max(int a, int b, int c);
int gen_as_op(int group, int opcode, int s_op, int d_op, int era);
int get_label_addr(char *label);
char *rm_from_left(char *line, int indx);
op_param *identify_param(char *param_word);
int add_to_assembled_list(int code);
int get_from_mllist(char *label);
void add_to_lbpr_list(int address, char *label);


int curr_op_addr;

int assemble_op(char *line)
{
	/*local vars*/
	int era = 0;
	op_param *s_operand = NULL;
	op_param *d_operand = NULL;
	int group;
	char *opc = "";
	int i = 0;
	int opcode_address = -1;
	
	/*1. get opcode*/
	while(!islower(line[i])){
		i++;
	}
	while(islower(line[i])){
		opc	 = appendc(opc,line[i]);
		i++;
	}
	struct oclist *opcode = getOpcodeByCode(opc);
	if(opcode != NULL){
		line = rm_from_left(line,i);
		i = 0;
	} else {
		/*error - opcode does not exist*/
	}
	while(line[i] == ' '){
		i++;
	}
	
	/*2. Identify if parameters are needed and their types*/
	if(opcode->numOfParams == 1){
		group = 1;
		char *param_word = "";
		while(line[i] != '\0'){
			param_word = appendc(param_word,line[i]);
			i++;
		}
		s_operand = identify_param(param_word);
		d_operand = NULL;
	} else if(opcode->numOfParams == 2){
		group = 2;
		char *s_param_word = "";
		char *d_param_word = "";
		if(line[i] == '~'){
			while(line[i] != ')'){
				s_param_word = appendc(s_param_word,line[i]);
				i++;
			}
			s_param_word = appendc(s_param_word,line[i]);
			i += 2; /*skip ( and , chars*/
		} else {
			while(line[i] != ',' && line[i] != '\0'){
				s_param_word = appendc(s_param_word,line[i]);
				i++;
			}
			i++;
		}
		s_operand = identify_param(s_param_word);
		if(line[i] == '~'){
			while(line[i] != ')'){
				d_param_word = appendc(d_param_word,line[i]);
				i++;
			}
			d_param_word = appendc(d_param_word,line[i]);
			i++;
		} else {
			while(line[i] != '\0'){
				d_param_word = appendc(d_param_word,line[i]);
				i++;
			}
			i++;
		}
		d_operand = identify_param(d_param_word);
	} else {
		group = 0;
		s_operand = d_operand = NULL;
	}
	if(group == 2 && s_operand->op_method == OP_METH_R_DIRECT && d_operand->op_method == OP_METH_R_DIRECT){
		add_to_assembled_list(assemble_param_rdirect(s_operand, d_operand, era));
	}
	
	/*add op to main counter*/
	int op_so = 0;
	int op_do = 0;
	if(s_operand != NULL){
		op_so = s_operand->op_method;
	}
	if(d_operand != NULL){
		op_do = d_operand->op_method;
	}
	int r_word = gen_as_op(group,opcode->val,op_so,op_do,era);
	printf("r_word = %X\n",r_word);
	curr_op_addr = opcode_address = add_to_assembled_list(r_word);
	
	if(s_operand != NULL){
		if(s_operand->op_method == OP_METH_R_DIRECT){
			add_to_assembled_list(assemble_param(s_operand,era, R_DIR_SOURCE));
		} else {
			int prm = assemble_param(s_operand,era, R_DIR_NONE);
			if(s_operand->op_method == OP_METH_DIRECT && prm == 0){
				/*inside the op_param you already have the label*/
				add_to_lbpr_list(add_to_assembled_list(prm), s_operand->param);
			} else {
				add_to_assembled_list(prm);
			}
		}
	}
	if(d_operand != NULL){
		if(d_operand->op_method == OP_METH_R_DIRECT){
			add_to_assembled_list(assemble_param(d_operand,era, R_DIR_DEST));
		} else {
			int prm = assemble_param(d_operand,era, R_DIR_NONE);
			if(d_operand->op_method == OP_METH_DIRECT && prm == 0){
				/*inside the op_param you already have the label*/
				add_to_lbpr_list(add_to_assembled_list(prm),d_operand->param);
			} else {
				add_to_assembled_list(prm);
			}
		}
	}
	
	return opcode_address;
}

op_param *identify_param(char *param_word)
{
	op_param *result = opp_alloc();
	result->op_method = -1;
	int era;
	if(param_word[0] == '#'){									
		result->op_method = OP_METH_INSTANT;
	} else if (param_word[0] == '~'){										   					
		result->op_method = OP_METH_DIST;
	} else if (param_word[0] == 'r'){	
		int r_addr;
		if((r_addr = get_r_addr(param_word)) != -1){
			result->op_method = OP_METH_R_DIRECT;
		}
	}
	if(result->op_method != OP_METH_INSTANT && result->op_method != OP_METH_DIST
	 && result->op_method != OP_METH_R_DIRECT){
		result->op_method = OP_METH_DIRECT;
	}
	result->param = param_word;
	return result;
}

int get_r_addr(char *name)
{
	int result = -1;
	if(strcmp(name,"r1") == 0){
		result = ADDR_R1;
	}
	if(strcmp(name,"r2") == 0){
		result = ADDR_R2;
	}
	if(strcmp(name,"r3") == 0){
		result = ADDR_R3;
	}
	if(strcmp(name,"r4") == 0){
		result = ADDR_R4;
	}
	if(strcmp(name,"r5") == 0){
		result = ADDR_R5;
	}
	if(strcmp(name,"r6") == 0){
		result = ADDR_R6;
	}
	if(strcmp(name,"r7") == 0){
		result = ADDR_R7;
	}
	if(strcmp(name,"r8") == 0){
		result = ADDR_R8;
	}
	return result;
}

int assemble_param(op_param *p, int era, int r_dir_side)
{
	printf("assemble_param p->param = %s, p->op_method = %d\n", p->param, p->op_method);
	int result = -1;
	if(p->op_method == OP_METH_INSTANT){
		printf("op meth instant\n");
		char *ptr = p->param;
		int num;
		int i = 0;
		while(ptr[i] != '\0'){
			if (isdigit(ptr[i])){
				num = ptr[i] - '0';
				i++;
				while (ptr[i] != '\0') {
					num *= 10;
					num += ptr[i] - '0';
					i++;
				}
			}
			i++;
		}
		num = num << 2;
		result = num | era;
	} else if(p->op_method == OP_METH_DIRECT) {
		result = get_from_mllist(p->param);
		printf("op meth direct\n");
	} else if(p->op_method == OP_METH_DIST){
		printf("op meth dist\n");
		char *label_one = "";
		char *label_two = "";
		int i = 0;
		while(p->param[i] != '\0'){
			if(isupper(p->param[i]) || islower(p->param[i])){
				while(p->param[i] != ','){
					label_one = appendc(label_one,p->param[i]);
					i++;
				}
				i++;
				while(p->param[i] != ')'){
					label_two = appendc(label_two,p->param[i]);
					i++;
				}
			}
			i++;
		}
		int lbl_one_addr = get_from_mllist(label_one);
		int lbl_two_addr = get_from_mllist(label_two);
		printf("in dist, '%s' = %d, '%s' = %d, curr_op_addr = %d\n",label_one,lbl_one_addr,label_two,lbl_two_addr,curr_op_addr);
		int max_addr = get_max_dist(lbl_one_addr,lbl_two_addr,curr_op_addr);
		max_addr = max_addr << 2;
		result = max_addr | era;
	} else if(p->op_method == OP_METH_R_DIRECT) {
		printf("op meth r direct\n");
		int r_addr = get_r_addr(p->param);
		if(r_dir_side == R_DIR_SOURCE){
			r_addr = r_addr << 7;
		} else if (r_dir_side == R_DIR_DEST){
			r_addr = r_addr << 2;
		} else {
			printf("you havent included the r_dir side in which to place the thing");
		}
		result = r_addr | era;
	}
	return result;
}

int assemble_param_rdirect(op_param *source_p, op_param *dest_p, int era)
{
	int r_addr_source = get_r_addr(source_p->param);
	int r_addr_dest = get_r_addr(dest_p->param);
	r_addr_source = r_addr_source << 7;
	r_addr_dest = r_addr_dest << 2;
	return (r_addr_source | r_addr_dest | era);
}

int get_max_dist(int addr_one, int addr_two, int op_addr)
{
	int dist_one_two = abs(addr_one-addr_two);
	int dist_op_one = abs(addr_one-op_addr);
	int dist_op_two = abs(addr_two-op_addr);
	return max(dist_one_two,dist_op_one,dist_op_two);
}

int max(int a, int b, int c)
{
	int max = a;
	if(b > a){
		max = b;
	}
	if(c > a){
		max = c;
	}
	return max;
}

int gen_as_op(int group, int opcode, int s_op, int d_op, int era)
{
	printf("generating opcode with group - %d, opcode - %d, s_op - %d, d_op - %d, era - %d\n", group, opcode, s_op, d_op, era);
	group = group << 10;
	opcode = opcode << 6;
	s_op = s_op << 4;
	d_op = d_op << 2;
	return (group | opcode | s_op | d_op | era);
}
