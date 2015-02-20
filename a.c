#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	FILE *f = fopen("testing","r");
	char c;
	while((c = fgetc(f)) != EOF){
		printf("havent reached EOF and letter is %c\n",c);
	}
	return 0;
}
