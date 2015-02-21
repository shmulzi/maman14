#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	main_llist = ll_append(label,address,main_llist);
}

int get_from_mllist(char *label)
{
	int result = -1;
	label_list *ptr = main_llist;
	for(; ptr != NULL;ptr = ptr->next){
		if(strcmp(ptr->label,label) == 0){
			result = ptr->address;
		}
	}
	return result;
}

void print_mllist()
{
	printf("--- List of Labels:\n");
	label_list *ptr;
	for(ptr = main_llist; ptr != NULL; ptr = ptr->next){
		printf("label name - %s            label address - %X\n",ptr->label,ptr->address);
	}
	printf("--- End list of Labels.\n");
}