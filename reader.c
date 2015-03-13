#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 80

/*genfunc.c*/
void print_error(char *err);
char *appendc(char *s, char c);
char *rm_from_left(char *line, int indx);
char *slice(char *s, int l_index, int r_index);

/*freadline - goes through a line in file f and if it hits an end of line / file it returns all the input it has gotten including the EOF if it found one for the line delegator to process
 * if the line is over 80 characters an error will appear and the assemble*/
char *freadline(FILE *f)
{
	char *line;
	char c;
	line = "";
	while((c = fgetc(f)) != '\n' && c != EOF){
		line = appendc(line,c);
	}
	if(c == EOF){
		line = appendc(line,c);
	}
	if(strlen(line) > MAX_LINE_LENGTH){
		print_error("General Error - Line length cannot be higher that 80 characters");
	}
	return line;
}