#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

void print_to_obj_file(char *fn, assembledlist *al)
{
	char *file_ext = ".obj";
	char *full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	FILE *f = fopen(full_fn,"w");
	assembledlist *ptr;
	for(ptr = al; ptr != NULL; ptr = ptr->next){
		fprintf(f,"%X        %X\n",ptr->address,ptr->word);
	}
}

void print_to_ent_file(char *fn, generic_list *ent_list)
{
	char *file_ext = ".ent";
	char *full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	FILE *f = fopen(full_fn,"w");
	generic_list *ptr;
	for(ptr = ent_list; ptr != NULL; ptr = ptr->next){
		fprintf(f,"%s        %X\n",ptr->label,ptr->address);
	}
}

void print_to_ext_file(char *fn, generic_list *ext_list)
{
	char *file_ext = ".ext";
	char *full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	FILE *f = fopen(full_fn,"w");
	generic_list *ptr;
	for(ptr = ext_list; ptr != NULL; ptr = ptr->next){
		fprintf(f,"%s        %X\n",ptr->label,ptr->address);
	}
}