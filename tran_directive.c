/*tran_directive.c - contains structures and functions that handle the assembly and commintment to memory of directives*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"

#define DIR_DOES_NOT_EXIST 0
#define DIR_DATA 1
#define DIR_STRING 2
#define DIR_ENTRY 3
#define DIR_EXTERN 4

/*self*/
int identify_dir(char *word);
int as_dataent(char *line);
int as_stringent(char *line);
void add_entry(char *label);
void add_extern(char *label);

/*main.c*/
int add_to_assembled_list(int code);

/*label.c*/
int get_from_mllist(char *label);

/*tran_op.c*/
int twos_complement_neg(int pos);

/*foutput.c*/
void print_to_ent_file(char *fn, generic_list *ent_list);
void print_to_ext_file(char *fn);

/*genfunc.c*/
void print_error(char *err);
char *appendc(char *s, char c);
char *rm_from_left(char *line, int indx);

generic_list *entry_list;
generic_list *extern_list;

generic_list *gnl_alloc(void)
{
    return (generic_list *)malloc(sizeof(generic_list));
}

/*append_opcode- recieves a generic list object and paramters for a new one, creates it and appends it to the last node
 * returns the recieved generic list object with the new node appended*/
generic_list *gnl_append(char *label, int address, generic_list *gnl)
{
    if (gnl == NULL) {
        gnl = gnl_alloc();
        gnl->label = label;
        gnl->address = address;
        gnl->next = NULL;
    } else {
        gnl->next = gnl_append(label,address,gnl->next);
    }
    return gnl;
}

/*isingnl - goes through generic list gnl and if the input label matches the label on one if its items it returns 1, if it does not contains the item it returns 0*/
int isingnl(char *label, generic_list *gnl)
{
	generic_list *ptr;
	int result = 0;
	for(ptr = gnl; ptr != NULL; ptr = ptr->next){
		if(strcmp(ptr->label,label) == 0){
			result = 1;
		}
	}
	return result;
}

/*isextern - looks through the extern list and if label matches of its items it returns 1, if it does not contain it - it returns 0*/
int isextern(char *label)
{
	return isingnl(label,extern_list);
}

/*datalist - a linked list for numbers that are gathered with a '.data' entry word*/
struct datalist {
    int num;
    struct datalist *next;
};


/*dl_alloc - allocates memory for a new item in the datalist linked list*/
struct datalist *dl_alloc(void)
{
    return (struct datalist *)malloc(sizeof(struct datalist));
}

/*dl_append - returns an datalist with an item added to it containing the word and address*/
struct datalist *dl_append(int num, struct datalist *dl)
{
    if (dl == NULL) {
        dl = dl_alloc();
        dl->num = num;
        dl->next = NULL;
    } else {
        dl->next = dl_append(num,dl->next);
    }
    return dl;
}

/*assemble_dir - goes through line, assembles a directive according to specifications, and puts in to the appropriate memory counters and lists*/
int assemble_dir(char *line) 
{
    
    int f_address;
    char *entry_word;
    int i;
    int entry_type;

    f_address = -1;
    entry_word = "";
    i = 0;

	while (line[i] == ' ') {
        i++;
    }

    while (line[i] != ' ') {
        entry_word = appendc(entry_word, line[i]);
        i++;
    }
    while (line[i] == ' ') {
        i++;
    }
    line = rm_from_left(line,i);
    entry_type = identify_dir(entry_word);
    if (entry_type == DIR_DATA) {
        f_address = as_dataent(line);
    } else if (entry_type == DIR_STRING) {
        f_address = as_stringent(line);
    } else if (entry_type == DIR_ENTRY) {
        add_entry(line);
    } else if (entry_type == DIR_EXTERN) {
        add_extern(line);
    } else if (entry_type == DIR_DOES_NOT_EXIST) {
        print_error("Directive does not exist");
    }
    return f_address;
}

/*identify_dir - identifies which directive, if at all, the word is and returns a predetermined value*/
int identify_dir(char *word)
{
    if (strcmp(word, ".data") == 0) {
        return DIR_DATA;
    }
    if (strcmp(word, ".string") == 0) {
        return DIR_STRING;
    }
    if (strcmp(word, ".entry") == 0) {
        return DIR_ENTRY;
    }
    if (strcmp(word, ".extern") == 0) {
        return DIR_EXTERN;
    }
    return DIR_DOES_NOT_EXIST;
}

/*as_dataent - assembles data entry according to the line and enters the integers input in to the main memory*/
int as_dataent(char *line)
{
    int f_address = -1;
    int i = 0;
    while (line[i] != '\0' && i < 11) {
        int num;
        int mult = 1;
        if (line[i] != '\0' && (isdigit(line[i]) || line[i] == '-')) {
			if(line[i] == '-'){
				mult *= -1;
				i++;
			}
            num = line[i] - '0';
            i++;
            while (line[i] != ',' && line[i] != '\0' && isdigit(line[i])) {
                num *= 10;
                num += line[i] - '0';
                i++;
            }
            if(mult < 0){
				num = twos_complement_neg(num);
			}
        } else {
        	print_error("Directive Error - .data directive can only include integers");
        }
        if(f_address == -1){
            f_address = add_to_assembled_list(num);
        } else {
			add_to_assembled_list(num);
		}
        if(line[i] != '\0'){
			i++;
		}
    }
    return f_address;
}

/*as_stringent - assembles a string directive according to the line, and enters the ascii values of the characters + a 0 value at the end to the memory*/
int as_stringent(char *line)
{
	int c;
    int f_address;
    int i; /*skipping one "*/

    f_address = -1;
    i = 1;

    while(line[i] != '"'){
    	if(line[i] == EOF || line[i] == '\n'){
    		print_error("Directive Error - .string directive must end with quotation marks");
    	}
        c = line[i];
        if(f_address == -1){
            f_address = add_to_assembled_list(c);
        } else {
			add_to_assembled_list(c);
		}
        i++;
    }
    add_to_assembled_list('\0');
    return f_address;
}

/*add_entry - adds entry to entry list according to label name*/
void add_entry(char *label)
{
    int address = get_from_mllist(label);
    entry_list = gnl_append(label,address,entry_list);
}

/*add_extern - adds extern to extern list according to label name*/
void add_extern(char *label)
{
    int address = get_from_mllist(label);
    extern_list = gnl_append(label,address,extern_list);
}

/*update_entries - goes through the entry list and searches the label list for the address of it, then updates it in the entry list*/
void update_entries()
{
	generic_list *ptr;
	for(ptr = entry_list; ptr != NULL; ptr = ptr->next){
		ptr->address = get_from_mllist(ptr->label);
	}
}

/*update_externs - goes through the extern list and searches the label list for the address of it, then updates it in the extern list*/
void update_externs()
{
	generic_list *ptr;
	for(ptr = extern_list; ptr != NULL; ptr = ptr->next){
		ptr->address = get_from_mllist(ptr->label);
	}
}

/*print_entry_list - prints the entry list in to the final .ent file*/
void print_entry_list(char *fn)
{
	generic_list *ptr;
	print_to_ent_file(fn, entry_list);
	printf("--- List of Entries:\n");
	for(ptr = entry_list; ptr != NULL; ptr = ptr->next){
		printf("Entry - %s           Address - %X\n",ptr->label,ptr->address);
	}
	printf("--- End list of Entries.\n");
}

/*print_extern_list - goes through the main lbpr list, and if the label is an extern it prints its name and address to the final .ext file*/
void print_extern_list(char *fn)
{
	generic_list *ptr;
	print_to_ext_file(fn);
	printf("--- List of Externs:\n");
	for(ptr = extern_list; ptr != NULL; ptr = ptr->next){
		printf("Entry - %s           Address - %X\n",ptr->label,ptr->address);
	}
	printf("--- End list of Externs.\n");
}
