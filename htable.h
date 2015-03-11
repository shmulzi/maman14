#include <stdio.h>
#include "defines.h"


struct slist
{
	char *word;
	int address;
	struct slist *next;
};
