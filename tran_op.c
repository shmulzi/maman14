#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "defines.h"

#define R_DIR_SOURCE 0
#define R_DIR_DEST 1
#define R_DIR_NONE 2

#define ADDR_R0 0
#define ADDR_R1 1
#define ADDR_R2 2
#define ADDR_R3 3
#define ADDR_R4 4
#define ADDR_R5 5
#define ADDR_R6 6
#define ADDR_R7 7

#define ABSOLUTE 0
#define EXTERN 1
#define RELOCATABLE 2

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

/*self*/
int get_r_addr(char *name);
int assemble_param(op_param *param, int r_dir_side);
int assemble_param_rdirect(op_param *source_p, op_param *dest_p, int era);
int get_max_dist(int addr_one, int addr_two, int op_addr);
int max(int a, int b, int c);
int gen_as_op(int group, int opcode, int s_op, int d_op, int era);
op_param *identify_param(char *param_word);
int is_dist_labels_addressed(char *dist_param);
int calc_dist(char *dist_param, int opcode_address);

/*opcodes.c*/
struct oclist *getOpcodeByCode(char *code);
int is_method_permitted(struct oclist *ocitem, int meth, int op_type);

/*main.c*/
int add_to_assembled_list(int code);

/*label.c*/
int get_from_mllist(char *label);

/*second_pass.c*/
void add_to_lbpr_list(int address, char *label);
void add_to_distpr_list(char *dist_param, int opcode_address, int address);

/*genfunc.c*/
void print_error(char *err);
char *appendc(char *s, char c);
char *rm_from_left(char *line, int indx);
char *slice(char *s, int l_index, int r_index);

/*global params*/
int curr_op_addr;

/*aseemble_op - receives a line string, identifies the opcode and its required params, assembles opcode and params and commits them to memory
 * returns the address for the opcode itself in the main memory*/
int assemble_op(char *line)
{
	/*local vars*/
	struct oclist *opcode;
	int op_so;
	int op_do;
	int r_word;
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
	opcode = getOpcodeByCode(opc);
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
		char *param_word;
		param_word = "";
		group = 1;
		while(line[i] != '\0'){
			param_word = appendc(param_word,line[i]);
			i++;
		}
		s_operand = NULL;
		d_operand = identify_param(param_word);	
		if(is_method_permitted(opcode,d_operand->op_method,OP_TYPE_DEST)){
			print_error("Op Error - Destination operand using illegal delivery method");
		}
	} else if(opcode->numOfParams == 2){
		char *s_param_word = "";
		char *d_param_word = "";
		group = 2;
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
		while(line[i] == ' ') { i++; }
		if(is_method_permitted(opcode,s_operand->op_method,OP_TYPE_SRC)){
			print_error("Op Error - Source operand using illegal delivery method");
		}
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
		if(is_method_permitted(opcode,d_operand->op_method,OP_TYPE_DEST)){
			print_error("Op Error - Destination operand using illegal delivery method");
		}
	} else {
		group = 0;
		s_operand = d_operand = NULL;
	}
	
	/*add op to main counter*/
	op_so = 0;
	op_do = 0;
	if(s_operand != NULL){
		op_so = s_operand->op_method;
	}
	if(d_operand != NULL){
		op_do = d_operand->op_method;
	}
	r_word = gen_as_op(group,opcode->val,op_so,op_do,ABSOLUTE);
	curr_op_addr = opcode_address = add_to_assembled_list(r_word);
	
	if(group == 2 && s_operand->op_method == OP_METH_R_DIRECT && d_operand->op_method == OP_METH_R_DIRECT){
		add_to_assembled_list(assemble_param_rdirect(s_operand, d_operand, ABSOLUTE));
	} else {
		if(s_operand != NULL){
			if(s_operand->op_method == OP_METH_R_DIRECT){
				add_to_assembled_list(assemble_param(s_operand, R_DIR_SOURCE));
			} else {
				int prm = assemble_param(s_operand,R_DIR_NONE);
				if(s_operand->op_method == OP_METH_DIRECT && prm == 2){
					add_to_lbpr_list(add_to_assembled_list(prm), s_operand->param);
				} else if (s_operand->op_method == OP_METH_DIST && is_dist_labels_addressed(s_operand->param)){
					add_to_distpr_list(s_operand->param,curr_op_addr,add_to_assembled_list(prm));
				} else {
					add_to_assembled_list(prm);
				}
			}
		}
		if(d_operand != NULL){
			if(d_operand->op_method == OP_METH_R_DIRECT){
				add_to_assembled_list(assemble_param(d_operand, R_DIR_DEST));
			} else {
				int prm = assemble_param(d_operand, R_DIR_NONE);
				if(d_operand->op_method == OP_METH_DIRECT && prm == 2){
					add_to_lbpr_list(add_to_assembled_list(prm),d_operand->param);
				} else if (d_operand->op_method == OP_METH_DIST && is_dist_labels_addressed(d_operand->param)){
					add_to_distpr_list(d_operand->param,curr_op_addr,add_to_assembled_list(prm));
				} else {
					add_to_assembled_list(prm);
				}
			}
		}
	}
	return opcode_address;
}

/*identify_param - recives a param word, identifies the param type and returns it as a param that can be assembled
 * returns an object of type op_param which contains the op type and the param itself for assembly*/
op_param *identify_param(char *param_word)
{
	int i;
	op_param *result = opp_alloc();
	result->op_method = -1;
	i = 0;
	
	while(param_word[i] == ' ') { i++; }
	
	if(param_word[i] == '#'){									
		result->op_method = OP_METH_INSTANT;
	} else if (param_word[i] == '~'){										   					
		result->op_method = OP_METH_DIST;
	} else if (param_word[i] == 'r'){	
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

/*is_dist_labels_addressed - recieves a distance param and checks if both labels inside of it are already addressed in the main memory
 * returns 1 if one of the labels do not have an address yet, 0 if not */
int is_dist_labels_addressed(char *dist_param)
{
	int result = 0;
	int i = 2; /*skip ~(*/
	char *paramone ="";
	char *paramtwo = "";
	while(dist_param[i] != ','){
		paramone = appendc(paramone,dist_param[i]);
		i++;
	}
	i++;
	while(dist_param[i] != ')'){
		paramtwo = appendc(paramtwo,dist_param[i]);
		i++;
	}
	if(get_from_mllist(paramone) == 0 || get_from_mllist(paramtwo) == 0){
		result = 1;
	}
	return result;
}

/*get_r_addr - recieves a string and checks if it matches the reserved name of a register, if so it returns its address
 * returns the address of the named register, or a -1 if the name is not resereved for a register*/
int get_r_addr(char *name)
{
	int result = -1;
	if(strcmp(name,"r0") == 0){
		result = ADDR_R0;
	} else if(strcmp(name,"r1") == 0){
		result = ADDR_R1;
	} else if(strcmp(name,"r2") == 0){
		result = ADDR_R2;
	} else if(strcmp(name,"r3") == 0){
		result = ADDR_R3;
	} else if(strcmp(name,"r4") == 0){
		result = ADDR_R4;
	} else if(strcmp(name,"r5") == 0){
		result = ADDR_R5;
	} else if(strcmp(name,"r6") == 0){
		result = ADDR_R6;
	} else if(strcmp(name,"r7") == 0){
		result = ADDR_R7;
	}
	return result;
}
/*twos_complement_neg - receives a positive integer and returns it as a two complement negative
 * returns an integer that is a twos complement negative of the positive number given*/
int twos_complement_neg(int pos)
{
	return ~pos+1;
}

/*assemble_param - recieves a parameter and an integer indicating on which side the register is in case it is a register (for register direct delivery method)
 * returns the address of the param after it is commited to the main memory*/
int assemble_param(op_param *p, int r_dir_side)
{
	int result = -1;
	if(p->op_method == OP_METH_INSTANT){
		char *ptr = p->param;
		int num;
		int i = 0;
		int mult = 1;
		i++; /*skip #*/
		if(ptr[i] == '-'){
			mult *= -1;
		}
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
		if(mult < 0){
			num = twos_complement_neg(num);
		}
		num = num << 2;
		result = num | ABSOLUTE;
	} else if(p->op_method == OP_METH_DIRECT) {
		result = (get_from_mllist(p->param) << 2) | RELOCATABLE;
	} else if(p->op_method == OP_METH_DIST){
		result = calc_dist(p->param, curr_op_addr);
	} else if(p->op_method == OP_METH_R_DIRECT) {
		int r_addr = get_r_addr(p->param);
		if(r_dir_side == R_DIR_SOURCE){
			r_addr = r_addr << 7;
		} else if (r_dir_side == R_DIR_DEST){
			r_addr = r_addr << 2;
		} else {
			printf("you havent included the r_dir side in which to place the thing");
		}
		result = r_addr | ABSOLUTE;
	}
	return result;
}

/*calc_dist - receives a distance param and its opcode address and returns the max distance between one param to the other, or each param to the opcode address
 * returns the max distance between the first and second label in the param, the distance between the opcode and the first label, and the distance between the opcode and the second label*/
int calc_dist(char *dist_param, int opcode_address)
{
	int lbl_one_addr;
	int lbl_two_addr;
	int result = -1;
	char *label_one = "";
	char *label_two = "";
	int i = 0;
	while(dist_param[i] != '\0'){
		if(isupper(dist_param[i]) || islower(dist_param[i])){
			while(dist_param[i] != ','){
				label_one = appendc(label_one,dist_param[i]);
				i++;
			}
			i++;
			while(dist_param[i] != ')'){
				label_two = appendc(label_two,dist_param[i]);
				i++;
			}
		}
		i++;
	}
	lbl_one_addr = get_from_mllist(label_one);
	lbl_two_addr = get_from_mllist(label_two);
	result = get_max_dist(lbl_one_addr,lbl_two_addr,opcode_address);
	return (result << 2) | ABSOLUTE;
}

/*assemble_param_rdirect - takes 2 params that are registers and assembles it as required and returns it
 * returns the assembled r direct param*/
int assemble_param_rdirect(op_param *source_p, op_param *dest_p, int era)
{
	int r_addr_source = get_r_addr(source_p->param);
	int r_addr_dest = get_r_addr(dest_p->param);
	r_addr_source = r_addr_source << 7;
	r_addr_dest = r_addr_dest << 2;
	return (r_addr_source | r_addr_dest | era);
}

/*get_max_dist - calculates the distance between the different addresses it receives and returns the max distance between them
 * returns the max distance between the 3 recieved addresses*/
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
	if(b > max){
		max = b;
	}
	if(c > max){
		max = c;
	}
	return max;
}

int gen_as_op(int group, int opcode, int s_op, int d_op, int era)
{
	group = group << 10;
	opcode = opcode << 6;
	s_op = s_op << 4;
	d_op = d_op << 2;
	return (group | opcode | s_op | d_op | era);
}
