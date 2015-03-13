#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void print_error(char *err)
{
	extern int curr_linenum;
	printf("Error in line %d - %s\n",curr_linenum,err);
	exit(-1);
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

char *rm_from_left(char *line, int indx)
{
	char *newline = "";
	int i = indx;
	while(line[i] != '\0'){
		newline = appendc(newline,line[i]);
		i++;
	}
	return newline;
}

char *rm_from_right(char *line, int indx)
{
	char *newline = "";
	int i = 0;
	while(line[i] != line[indx]){
		newline = appendc(newline,line[i]);
		i++;
	}
	return newline;
}

char *slice(char *s, int l_index, int r_index)
{
	char *result = "";
	int i;
	for(i = l_index; i <= r_index;i++){
		result = appendc(result,s[i]);
	}
	return result;
}