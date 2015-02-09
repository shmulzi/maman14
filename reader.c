#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 80

#define INSTRUCT_DOES_NOT_EXIST 0
#define INSTRUCT_DATA 1
#define INSTRUCT_STRING 2
#define INSTRUCT_ENTRY 3
#define INSTRUCT_EXTERN 4

#define MEM_PTR_START_POINT 100

char *appendc(char *s, char c);
char *fgetword(FILE *f, char fc);
void fident(FILE *f, char *word, char fc);
void fas_dataent(FILE *f);
void fas_stringent(FILE *f);

/*assembledlist - a linked list that contains the final memory - word counter to print*/
typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

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

/*readline - reads a line in the File f and returns 0 if it finished correctly*/
int readline(FILE *f)
{
	char c;
	while((c = fgetc(f)) != '\n' && c != EOF) {
		if(c == ';'){
			/*this line is a comment*/
			return 0;
		}
		
		if (c == '.'){
			/*this line is a insrtuction line*/
			char *word = fgetword(f,c);
            fident(f,word,c);
			printf("\nInstruction word received in line - %s\n", word);
            exit(0);
		}
		
		if (isupper(c) || islower(c)){
			/*this line is either a tagged action / insrtuction line, or an action line*/
			char *word = fgetword(f,c);
			printf("\nregular word received in line - %s\n", word);
		} 
		
	}
	return 0;
}

/*appendc - adds the character c to the string s and returns it with the added character*/
char *appendc(char *s, char c)
{
	char *str;
	
	size_t len = strlen(s);
	str = (char *)malloc(len+2); /*adding extra space for the character and end string char '\0' */
	strcpy(str,s);
	str[len] = c;
	str[len + 1] = '\0';
	
	return str;
	
}

/*fgetword - reads a word from the pointer fc and until it is ended by a space, EOF or endline*/
char *fgetword(FILE *f, char fc)
{
	char *word = "";
	char c;
	word = appendc(word,fc);
	while(((c = fgetc(f)) != '\n') && c != EOF && c != ' ')
	{
		word = appendc(word,c);	
	}
	return word;		
}

void fident(FILE *f,char *word, char fc)
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

int main()
{
	/*FILE *f = fopen("test.txt","r");
	readline(f);*/
	assembledlist *tempAl = NULL;
}
