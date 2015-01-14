#include <stdio.h>


void printInBinary(int num)
{
	
	for(;num != 0; num = num / 2)
	{
		int b;
		b = num % 2;
		putchar(b + '0');
	}
	
}

void printInHex(int num)
{
	printf("%X", num);
}
