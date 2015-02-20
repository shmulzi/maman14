#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define INSTRUCT_DOES_NOT_EXIST 0
#define INSTRUCT_DATA 1
#define INSTRUCT_STRING 2
#define INSTRUCT_ENTRY 3
#define INSTRUCT_EXTERN 4

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

void fident(FILE *f,char *word)
{
    if (strcmp(word, ".data") == 0) {
        fas_dataent(f);
    }
    if (strcmp(word, ".string") == 0) {
		fas_stringent(f);
    }
    if (strcmp(word, ".entry") == 0) {
        printf("\nEntry data word found\n");
    }
    if (strcmp(word, ".extern") == 0) {
        printf("\nInstruct data word found\n");
    }
    printf("\nError - not a valid data entry word\n");
}

void fas_dataent(FILE *f)
{
    char c;
    struct datalist *data_entries = NULL;
    while (((c = fgetc(f)) != '\n') && c != EOF) {
        int i;
        if (isdigit(c)) {
            i = c - '0';
            while (((c = fgetc(f)) != ',') && c != EOF && isdigit(c)) {
                i *= 10;
                i += c - '0';
            }
        }
        if (c == ',' || c == EOF || c == '\n') {
            data_entries = dl_append(i, data_entries);
        }
    }
    for(;data_entries != NULL; data_entries = data_entries->next){
		printf("\na number from entries - %d\n", data_entries->num);
	}
}

void fas_stringent(FILE *f)
{
	char *word = "";
	char c;
	while((c = fgetc(f)) != '"');
	while((c = fgetc(f)) != '"')
	{
		word = appendc(word,c);	
	}
	printf("\nin fas_stringent the word is %s\n", word);
}
