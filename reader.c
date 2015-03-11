#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 80

/*self*/
char *appendc(char *s, char c);
char *rm_from_left(char *line, int indx);
char *rm_from_right(char *line, int indx);
char *slice(char *s, int l_index, int r_index);

/*label.c*/
void add_to_mllist(char *label, int word);

/*tran_directive.c*/
int assemble_dir(char *line);

/*tran_op.c*/
int assemble_op(char *line);

/*main.c*/
void print_error(char *err);


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

int delegate_line(char *line)
{
	int r;
	char *label;
	int is_dir;
	int label_addr;
	int i;
	r = 0;
	label = "";
	is_dir = 0;
	label_addr = 0;
	if(strlen(line) > 0){
		if(line[0] == ';'){
			/*This is a comment, go to next line*/
			return 0;
		}
		if(line[strlen(line)-1] == EOF){
			line = rm_from_right(line,strlen(line)-1);
			r = 1;
		}
		i = 0;
		while(line[i] != '\0'){
			if(line[i] == ':'){
				label = slice(line,0,i-1);
				line = rm_from_left(line, i+1);
				i = 0;
				while(line[i] == ' '){ i++; }
			}
			if(line[i] == '.'){
				label_addr = assemble_dir(line);
				is_dir = 1;
			}
			i++;
		}
		
		if(is_dir == 0){
			label_addr = assemble_op(line);
		}
		if(strlen(label) > 0){
			add_to_mllist(label,label_addr);
		}
	}
	return r;
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


