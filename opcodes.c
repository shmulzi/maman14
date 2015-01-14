#include "exampleheader.h"
#include <stdlib.h>

struct oclist *oclistAlloc(void);

struct oclist
{
	char *code;
	int numOfParams;
	int val;
	struct oclist *next;
};

struct oclist *opcodes;

struct oclist *addOpcode(char *code, int numOfParams, int val, struct oclist *ocl)
{
	if(ocl == NULL)
	{
		ocl = oclistAlloc();
		ocl->code = code;
		ocl->numOfParams = numOfParams;
		ocl->val = val;
		ocl->next = NULL;
	} else {
		ocl->next = addOpcode(code, numOfParams, val, ocl->next);
	}
	return ocl;
}

void printOpcodeList(){
	struct oclist *ptr;
	ptr = opcodes;
	printf("\nstarting print: \n");
	printf("\ncode:	numOfParams:	val: \n");
	for(; ptr != NULL; ptr = ptr->next)
	{
		printf("%s	%d	%d\n", ptr->code, ptr->numOfParams, ptr->val);
	}
}

struct oclist *getOpcodeByCode(char *code)
{
	struct oclist *ptr;
	
	if(opcodes != NULL)
	{
		for(ptr = opcodes; ptr != NULL; ptr = ptr->next)
		{
			if(strcmp(code, ptr->code) == 0)
			{
				return ptr;
			}
		}
	} else {
		printf("opcodes.c - opcodes table has failed to populate\n");
	}
	return ptr;
}

void populateOclist()
{
	opcodes = addOpcode("mov", 2, OPCODE_MOV, opcodes);
	opcodes = addOpcode("cmp", 2, OPCODE_CMP, opcodes);
	opcodes = addOpcode("add", 2, OPCODE_ADD, opcodes);
	opcodes = addOpcode("sub", 2, OPCODE_SUB, opcodes);
	opcodes = addOpcode("not", 1, OPCODE_NOT, opcodes);
	opcodes = addOpcode("clr", 1, OPCODE_CLR, opcodes);
	opcodes = addOpcode("lea", 2, OPCODE_LEA, opcodes);
	opcodes = addOpcode("inc", 1, OPCODE_INC, opcodes);
	opcodes = addOpcode("dec", 1, OPCODE_DEC, opcodes);
	opcodes = addOpcode("jmp", 1, OPCODE_JMP, opcodes);
	opcodes = addOpcode("bne", 1, OPCODE_BNE, opcodes);
	opcodes = addOpcode("red", 1, OPCODE_RED, opcodes);
	opcodes = addOpcode("prn", 1, OPCODE_PRN, opcodes);
	opcodes = addOpcode("jsr", 1, OPCODE_JSR, opcodes);
	opcodes = addOpcode("rts", 0, OPCODE_RTS, opcodes);
	opcodes = addOpcode("stop", 0, OPCODE_STOP, opcodes);
	
}

struct oclist *oclistAlloc(void)
{
	return (struct oclist *) malloc(sizeof(struct oclist));
}
