#include <stdio.h>
#include <string.h>


#define MAX_LINE_LENGTH 80

char *appendc(char *s, char c);
char *fgetword(FILE *f, char fc);

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
			printf("\nInstruction word received in line - %s\n", word);
			
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

int main()
{
	FILE *f = fopen("test.txt","r");
	readline(f);
}
