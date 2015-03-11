#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"

/*tran_directive.c*/
int isextern(char *label);

/*second_pass.c*/
lbpr_list *get_main_lbpr_list();

/*print_to_obj_file - prints an assembled list to the obj file*/
void print_to_obj_file(char *fn, assembledlist *al)
{
	char *file_ext;
	char *full_fn;
	FILE *f;
	assembledlist *ptr;
	file_ext = ".obj";
	full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	f = fopen(full_fn,"w");
	fprintf(f,"Base 16 Address        Base 16 machine code\n");
	for(ptr = al; ptr != NULL; ptr = ptr->next){
		fprintf(f,"%X        %X\n",ptr->address,ptr->word);
	}
}

/*print to ent file - prints an entry list to the .ent file*/
void print_to_ent_file(char *fn, generic_list *ent_list)
{
	char *file_ext;
	char *full_fn;
	FILE *f;
	generic_list *ptr;
	file_ext = ".ent";
	full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	f = fopen(full_fn,"w");
	for(ptr = ent_list; ptr != NULL; ptr = ptr->next){
		fprintf(f,"%s        %X\n",ptr->label,ptr->address);
	}
}

/*print to ext file - goes through the label list and compares to the extern file*/
void print_to_ext_file(char *fn)
{
	char *file_ext;
	char *full_fn;
	FILE *f;
	lbpr_list *ptr;
	file_ext = ".ext";
	full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	f = fopen(full_fn,"w");
	for(ptr = get_main_lbpr_list(); ptr != NULL; ptr = ptr->next){
		if(isextern(ptr->label)){
			fprintf(f,"%s        %X\n",ptr->label,ptr->address);
		}
	}
}
