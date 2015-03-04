#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

void print_to_obj_file(char *fn, assembledlist *al)
{
	char *file_ext = ".obj";
	char *full_fn = malloc(strlen(fn) + strlen(file_ext) + 1);
	strcpy(full_fn,fn);
	strcat(full_fn,file_ext);
	FILE *f = fopen(full_fn,"w");
	assembledlist *ptr;
	for(ptr = al; ptr != NULL; ptr = ptr->next){
		printf("printing to file - %X, %X",ptr->address,ptr->word);
		fprintf(f,"%X        %X\n",ptr->address,ptr->word);
	}
}
