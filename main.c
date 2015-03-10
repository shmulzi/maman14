#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MEM_PTR_START_POINT 100

/*reader.c*/
char *freadline(FILE *f);
int delegate_line(char *line);

/*opcodes.c*/
void populateOclist();
void printOpcodeList();

/*label.c*/
void print_mllist();

/*tran_directive.c*/
void print_entry_list();
void print_extern_list();

/*second_pass.c*/
void update_paramter_label_addresses();
void print_lbpr();

/*global variables*/
int curr_linenum;
int main_address;
char *curr_err;

/*assembledlist - a linked list that contains the final memory - word counter to print*/
typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

typedef struct genlist{
    char *label;
    int address;
    struct genlist *next;
} generic_list;

void print_to_obj_file(char *fn, assembledlist *al);

assembledlist *main_mem;

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

void update_word_at_address(int address, int word)
{
	assembledlist *ptr;
	for(ptr = main_mem; ptr != NULL; ptr = ptr->next){
		if(ptr->address == address){
				ptr->word = word;
		}
	}
}

void print_main_al()
{
	printf("--- List of Assembled:\n");
	for(; main_mem != NULL; main_mem = main_mem->next){
		printf("Word Address - %X           Word - %X\n",main_mem->address,main_mem->word);
	}
	printf("--- End list of Assembled.\n");
}

void update_curr_err(char *err)
{
	curr_err = err;
}

void print_error(char *err)
{
	printf("Error in line %d - %s\n",curr_linenum,err);
	exit(-1);
}

int hasasext(char *fn)
{
	int i = strlen(fn) - 3;
	if(fn[i] != '.'){
		return 0;
	}
	i++;
	if(fn[i] != 'a'){
		return 0;
	}
	i++;
	if(fn[i] != 's'){
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int i;
	curr_err = "";
	main_address = MEM_PTR_START_POINT;
	main_mem = NULL;
	populateOclist();
	for(i = 1; i < argc;i++){
		char *file_ext = ".as";
		char *full_fn = malloc(strlen(argv[i]) + strlen(file_ext) + 1);	
		strcpy(full_fn,argv[i]);
		strcat(full_fn,file_ext);
		FILE *f;
		int read_switch = 0;
		curr_linenum = 1;
		if((f = fopen(full_fn,"r")) != NULL){
			while(read_switch == 0){
				read_switch = delegate_line(freadline(f));
				curr_linenum++;
			}
			printf("printing from as file %s\n",full_fn);
			print_to_obj_file(argv[i],main_mem);
		} else {
			printf("Preassembly error - the file %s does not exist\n",full_fn);
		}
	}
	update_paramter_label_addresses();
	
	/*printf("\n--->Begin Report of Data Collected - \n\n");

	if(main_mem != NULL){
		print_mllist();
		printf("\n");
		print_main_al();
		printf("\n");
		print_entry_list(argv[1]);
		printf("\n");
		print_extern_list(argv[1]);
	}
	printf("\n--->Finished Report of Data Collected\n\n");
	printOpcodeList();
	printf("\n");
	print_lbpr();*/
	return 0;
}
