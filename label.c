#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LABEL_SIZE 30

/*opcodes.c*/
int isopcode(char *code);

/*genfunc.c*/
void print_error(char *err);

typedef struct llist{
	char *label;
	int address;
	struct llist *next;
} label_list;

label_list *main_llist;

label_list *ll_alloc(void)
{
	return (label_list *)malloc(sizeof(label_list));
}

label_list *ll_append(char *label, int address, label_list *ll)
{
	if(ll == NULL){
		ll = ll_alloc();
		ll->label = label;
		ll->address = address;
		ll->next = NULL;
	} else {
		ll->next = ll_append(label,address,ll->next);
	}
	return ll;
}

void add_to_mllist(char *label, int address)
{
	if(strlen(label) > MAX_LABEL_SIZE){
		print_error("Label Error - Label size cannot be above 30 characters");
	} else if(!isopcode(label)) {
		print_error("Label Error - Label cannot have same name as opcode");
	} else {
		main_llist = ll_append(label,address,main_llist);
	}
}

int get_from_mllist(char *label)
{
	int result;
	label_list *ptr;
	result = 0;
	ptr = main_llist;
	for(; ptr != NULL;ptr = ptr->next){
		if(strcmp(ptr->label,label) == 0){
			result = ptr->address;
		}
	}
	return result;
}

void print_mllist()
{
	label_list *ptr;
	printf("--- List of Labels:\n");
	for(ptr = main_llist; ptr != NULL; ptr = ptr->next){
		printf("label name - %s            label address - %X\n",ptr->label,ptr->address);
	}
	printf("--- End list of Labels.\n");
}
