#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*print_error - prints out the received string and exits the program with a result -1*/
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

/*rm_from_left - takes in a string and returns it with characters removed from the far left up to index
 * retruns the result string*/
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
/*rm_from_right - takes in a string and returns it with characters removed from the far right up to index
 * retruns the result string*/
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

/*slice - takes in a string and returns a substring from r_index to l_index
 * returns the result string*/
char *slice(char *s, int l_index, int r_index)
{
	char *result = "";
	int i;
	for(i = l_index; i <= r_index;i++){
		result = appendc(result,s[i]);
	}
	return result;
}
