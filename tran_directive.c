#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIR_DOES_NOT_EXIST 0
#define DIR_DATA 1
#define DIR_STRING 2
#define DIR_ENTRY 3
#define DIR_EXTERN 4

char *rm_from_left(char *line, int indx);
char *rm_from_right(char *line, int indx);
int identify_dir(char *word);
void as_dataent(char *line);
void as_stringent(char *line);
int add_to_assembled_list(int code);
int get_from_mllist(char *label);
void add_entry(char *label);
void add_extern(char *label);

typedef struct genlist{
    char *label;
    int address;
    struct genlist *next;
} generic_list;

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

int assemble_dir(char *line) {
    
    int result = -1;
    char *entry_word = "";
    int i = 0;

    while (line[i] != ' ') {
        entry_word = appendc(entry_word, line[i]);
        i++;
    }
    while (line[i] == ' ') {
        i++;
    }
    int entry_type = identify_dir(entry_word);
    if (entry_type == DIR_DATA) {
        as_dataent(line);
    } else if (entry_type == DIR_STRING) {
        as_stringent(line);
    } else if (entry_type == DIR_ENTRY) {
        add_entry(line);
    } else if (entry_type == DIR_EXTERN) {
        add_extern(line);
    } else if (entry_type == DIR_DOES_NOT_EXIST) {
        
    }

    return result;
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
    while (line[i] != '\n' && line[i] != EOF) {
        int num;
        if (isdigit(line[i])) {
            num = line[i] - '0';
            while (line[i] != ',' && line[i] != EOF && isdigit(line[i])) {
                num *= 10;
                num += line[i] - '0';
                i++;
            }
        }
        if(f_address == -1){
            f_address = add_to_assembled_list(num);
        }
        
        i++;
    }
    return f_address;
}

int as_stringent(char *line)
{
    int f_address = -1;
    int i = 1; /*skipping one "*/
    while(line[i] != '"'){
        int c = line[i];
        if(f_address == -1){
            f_address = add_to_assembled_list(c);
        }
        i++;
    }
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
    entry_extern = gnl_append(label,address,entry_extern);
}