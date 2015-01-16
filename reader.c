#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 80

#define INSTRUCT_DOES_NOT_EXIST 0
#define INSTRUCT_DATA 1
#define INSTRUCT_STRING 2
#define INSTRUCT_ENTRY 3
#define INSTRUCT_EXTERN 4

char *appendc(char *s, char c);
char *fgetword(FILE *f, char fc);
int fident(FILE *f,char *word);
void fas_dataent(FILE *f);


struct datalist {
    int num;
    struct datalist *next;
};

struct datalist *dl_alloc(void)
{
    return (struct datalist *)malloc(sizeof(struct datalist));
}

struct datalist *dl_append(int num, struct datalist *dl)
{
    if (dl == NULL) {
        printf("\nentering first num in datalist\n");
        dl = dl_alloc();
        dl->num = num;
        dl->next = NULL;
    } else {
        printf("\nentering num in datalist - dl->num is %d\n", dl->num);
        dl->next = dl_append(num,dl->next);
    }
    return dl;
}

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
            fident(f,word);
			printf("\nInstruction word received in line - %s\n", word);
            exit(0);
			
		}
		
		if (c >= 'A' || c <= 'z'){
			/*this line is either a tagged action / insrtuction line, or an action line*/
			char *word = fgetword(f,c);
			printf("\nregular word received in line - %s\n", word);
		} 
		
	}
	return 0;
}

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

int fident(FILE *f,char *word)
{
    if (strcmp(word, ".data") == 0) {
        fas_dataent(f);
        return INSTRUCT_DATA;
    }
    if (strcmp(word, ".string") == 0) {
        return INSTRUCT_STRING;
    }
    if (strcmp(word, ".entry") == 0) {
        return INSTRUCT_ENTRY;
    }
    if (strcmp(word, ".extern") == 0) {
        return INSTRUCT_EXTERN;
    }
    return INSTRUCT_DOES_NOT_EXIST;
}

void fas_dataent(FILE *f)
{
    char c;
    struct datalist *data_entries = NULL;
    while (((c = fgetc(f)) != '\n') && c != EOF) {
        int i;
        if (isdigit(c)) {
            printf("\nwent in to isdigit and c is %c\n", c);
            i = c - '0';
            printf("\nwand now i is %d\n", i);
            while (((c = fgetc(f)) != ',') && c != EOF && isdigit(c)) {
                printf("\nwent to perform changes on i - only if it has more than one digit - c is now %c and i is now %d\n", c, i);
                i *= 10;
                i += c - '0';
                printf("\ni is now %d\n", i);
            }
        }
        printf("\nwand now now i is %d\n", i);
        if (c == ',' || c == EOF || c == '\n') {
            printf("\nwent in to enter the number %d and sizeof entries is %lu while sizeof int is always %lu\n", i, sizeof(data_entries), sizeof(int));
            data_entries = dl_append(i, data_entries);
            printf("\nfinished entering data_entry\n");
        }
        printf("\nfinished loop\n");
    }
}



int main()
{
	FILE *f = fopen("test.txt","r");
	readline(f);
}
