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

/*assembledlist - a linked list that contains the final memory - word counter to print*/
typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

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

/*printAssembledList - prints out the addresses and words in the given assembledlist*/
void printAssembledList(assembledlist *al)
{
	if(al->next == NULL){
		printf("%X         %X\n",al->address,al->word);
	} else {
		printf("%X         %X\n",al->address,al->word);
		printAssembledList(al->next);
	}
}

/*add_to_assembled_list - provides a way for all delegates functions to add code to the assembled list, and handles assigining the address*/
int add_to_assembled_list(int code)
{
	if(main_mem != NULL){
		main_mem = al_append(code,(main_mem->address++),main_mem);
	} else {
		main_mem = al_append(code,MEM_PTR_START_POINT,main_mem);
	}
	return main_mem->address;
}

void print_main_al()
{
	printf("--- List of Assembled:\n");
	assembledlist *ptr;
	for(ptr = main_mem; ptr != NULL; ptr = ptr->next){
		printf("Word Address - %X           Word - %X\n",ptr->address,ptr->word);
	}
	printf("--- End list of Assembled.\n");
}

int main(int argc, char *argv[])
{ 
	populateOclist();
	if(argc == 2){
		FILE *f = fopen(argv[1],"r");
		int read_switch = 0;
		while(read_switch == 0){
			read_switch = delegate_line(freadline(f));
		}
	} else {
		printf("you can only enter one argument currently\n");
	}
	if(main_mem != NULL){
		printf("%d\n",main_mem->address);
	}
	print_mllist();
	print_main_al();
	print_entry_list();
	print_extern_list();
	return 0;
}
