#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MEM_PTR_START_POINT 100


char *freadline(FILE *f);
int delegate_line(char *line);
void populateOclist();
void print_mllist();
void print_entry_list();
void print_extern_list();
void update_paramter_label_addresses();

int curr_linenum;
int main_address;
char *curr_err;

/*assembledlist - a linked list that contains the final memory - word counter to print*/
typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

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
		printf("al_append - al->word = %X, al->address = %X\n",al->word,al->address);
	} else {
		al->next = al_append(word,address,al->next);
	}
	return al;
}

/*add_to_assembled_list - provides a way for all delegates functions to add code to the assembled list, and handles assigining the address*/
int add_to_assembled_list(int code)
{
	printf("adding to assembled list - %X\n", code);
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

int main(int argc, char *argv[])
{
	curr_err = "";
	main_address = MEM_PTR_START_POINT;
	main_mem = NULL;
	populateOclist();
	if(argc == 2){
		FILE *f = fopen(argv[1],"r");
		int read_switch = 0;
		curr_linenum = 1;
		while(read_switch == 0){
			printf("------------------------- begin line %d ----------------------------------\n",curr_linenum);
			read_switch = delegate_line(freadline(f));
			printf("------------------------- end line %d ----------------------------------\n",curr_linenum);
			curr_linenum++;
		}
	} else {
		printf("you can only enter one argument currently\n");
	}
	update_paramter_label_addresses();

	print_to_obj_file(argv[1],main_mem);
	printf("\n--->Begin Report of Data Collected - \n\n");

	if(main_mem != NULL){
		print_mllist();
		printf("\n");
		print_main_al();
		printf("\n");
		print_entry_list();
		printf("\n");
		print_extern_list();
	}
	printf("\n--->Finished Report of Data Collected\n\n");
	return 0;
}
