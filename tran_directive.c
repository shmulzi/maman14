#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DIR_DOES_NOT_EXIST 0
#define DIR_DATA 1
#define DIR_STRING 2
#define DIR_ENTRY 3
#define DIR_EXTERN 4

char *appendc(char *s, char c);
char *rm_from_left(char *line, int indx);
int identify_dir(char *word);
int as_dataent(char *line);
int as_stringent(char *line);
int add_to_assembled_list(int code);
int get_from_mllist(char *label);
void add_entry(char *label);
void add_extern(char *label);
int get_from_mllist(char *label);
void print_error(char *err);

typedef struct genlist{
    char *label;
    int address;
    struct genlist *next;
} generic_list;

void print_to_ent_file(char *fn, generic_list *ent_list);
void print_to_ext_file(char *fn, generic_list *ext_list);

generic_list *entry_list;
generic_list *extern_list;

generic_list *gnl_alloc(void)
{
    return (generic_list *)malloc(sizeof(generic_list));
}

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

int as_dataent(char *line)
{
    int f_address = -1;
    int i = 0;
    while (line[i] != '\0' && i < 11) {
        int num;
        if (line[i] != '\0' && isdigit(line[i])) {
            num = line[i] - '0';
            i++;
            while (line[i] != ',' && line[i] != '\0' && isdigit(line[i])) {
                num *= 10;
                num += line[i] - '0';
                i++;
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

void add_entry(char *label)
{
    int address = get_from_mllist(label);
    entry_list = gnl_append(label,address,entry_list);
}

void add_extern(char *label)
{
    int address = get_from_mllist(label);
    extern_list = gnl_append(label,address,extern_list);
}

void update_entries()
{
	generic_list *ptr;
	for(ptr = entry_list; ptr != NULL; ptr = ptr->next){
		ptr->address = get_from_mllist(ptr->label);
	}
}

void update_externs()
{
	generic_list *ptr;
	for(ptr = extern_list; ptr != NULL; ptr = ptr->next){
		ptr->address = get_from_mllist(ptr->label);
	}
}

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

void print_extern_list(char *fn)
{
	generic_list *ptr;
	print_to_ext_file(fn,extern_list);
	printf("--- List of Externs:\n");
	for(ptr = extern_list; ptr != NULL; ptr = ptr->next){
		printf("Entry - %s           Address - %X\n",ptr->label,ptr->address);
	}
	printf("--- End list of Externs.\n");
}
