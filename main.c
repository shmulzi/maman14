/*main.c - contains the main function for the program, along with the main memory and a function to delegeate each line in the received file to proper assembly*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

#define MEM_PTR_START_POINT 100

/*reader.c*/
char *freadline(FILE *f);

/*opcodes.c*/
void populate_oclist();

/*second_pass.c*/
void update_paramter_label_addresses();

/*label.c*/
void add_to_mllist(char *label, int word);

/*tran_directive.c*/
int assemble_dir(char *line);

/*tran_op.c*/
int assemble_op(char *line);

/*genfunc.c*/
void print_error(char *err);
char *appendc(char *s, char c);
char *rm_from_left(char *line, int indx);
char *rm_from_right(char *line, int indx);
char *slice(char *s, int l_index, int r_index);

/*global variables*/
int curr_linenum;
int main_address;

/*foutput.c*/
void print_to_obj_file(char *fn, assembledlist *al);

assembledlist *main_mem;

int op_count;
int dir_count;

/*al_alloc - allocates memory for a new item in the assembledlist linked list*/
assembledlist *al_alloc(void)
{
	return (assembledlist *)malloc(sizeof(assembledlist));
}

/*al_append - returns an assembledlist with an item added to it containing the word and address*/
assembledlist *al_append(int word, int address, assembledlist *al)
{
	if(al == NULL){
		al = al_alloc();
		al->word = word;
		al->address = address;
		al->next = NULL;
	} else {
		al->next = al_append(word,address,al->next);
	}
	return al;
}

/*add_to_assembled_list - provides a way for all delegates functions to add code to the assembled list, and handles assigining the address*/
int add_to_assembled_list(int code)
{
	main_mem = al_append(code,main_address,main_mem);
	main_address++;
	return main_address-1;
}

/*update_word_at_address - finds the location to update according to the address, and replaces its content with the input word*/
void update_word_at_address(int address, int word)
{
	assembledlist *ptr;
	for(ptr = main_mem; ptr != NULL; ptr = ptr->next){
		if(ptr->address == address){
				ptr->word = word;
		}
	}
}

/*delegate_line - recieves a line, analyzes it and delegates it accordingly (creates label if nescesary, and moves the outcome to either create an op or a directive)
 * returns 1 if EOF was in the line or 0 if not*/
int delegate_line(char *line)
{
	int r;
	char *label;
	int is_dir;
	int label_addr;
	int i;
	r = 0;
	label = "";
	is_dir = 0;
	label_addr = 0;
	if(strlen(line) > 0){
		if(line[0] == ';'){
			/*This is a comment, go to next line*/
			return 0;
		}
		if(line[strlen(line)-1] == EOF){
			line = rm_from_right(line,strlen(line)-1);
			r = 1;
		}
		i = 0;
		while(line[i] != '\0'){
			if(line[i] == ':'){
				label = slice(line,0,i-1);
				line = rm_from_left(line, i+1);
				i = 0;
				while(line[i] == ' '){ i++; }
			}
			if(line[i] == '.'){
				label_addr = assemble_dir(line);
				is_dir = 1;
				dir_count++;
			}
			i++;
		}
		if(is_dir == 0){
			label_addr = assemble_op(line);
			op_count++;
		}
		if(strlen(label) > 0){
			add_to_mllist(label,label_addr);
		}
	}
	return r;
}

int main(int argc, char *argv[])
{
	int i;
	main_address = MEM_PTR_START_POINT;
	main_mem = NULL;
	op_count = dir_count = 0;
	populate_oclist();
	for(i = 1; i < argc;i++){
		FILE *f;
		int read_switch = 0;
		char *file_ext = ".as";
		char *full_fn = malloc(strlen(argv[i]) + strlen(file_ext) + 1);	
		strcpy(full_fn,argv[i]);
		strcat(full_fn,file_ext);
		curr_linenum = 1;
		if((f = fopen(full_fn,"r")) != NULL){
			while(read_switch == 0){
				read_switch = delegate_line(freadline(f));
				curr_linenum++;
			}
			printf("printing from as file %s\n",full_fn);
			update_paramter_label_addresses();
			print_to_obj_file(argv[i],main_mem);
		} else {
			printf("Preassembly error - the file %s does not exist\n",full_fn);
		}
	}
	return 0;
}
