#include "exampleheader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct oclist *oclistAlloc(void);

struct oclist
{
	char *code;
	int numOfParams;
	int val;
	int leg_sop[4];
	int leg_dop[4];
	struct oclist *next;
};

struct oclist *opcodes;

void arrcpy(int srcarr[4], int *dstarr[4])
{
	int i;
	for(i = 0; i < sizeof(srcarr); i++){
		dstarr[i] = srcarr[i];
	}
}

struct oclist *addOpcode(char *code, int numOfParams, int val, int leg_sop[4], int leg_dop[4], struct oclist *ocl)
{
	if(ocl == NULL)
	{
		ocl = oclistAlloc();
		ocl->code = code;
		ocl->numOfParams = numOfParams;
		ocl->val = val;
		arrcpy(leg_sop, ocl->leg_sop);
		arrcpy(leg_dop, ocl->leg_dop);
		ocl->next = NULL;
	} else {
		ocl->next = addOpcode(code, numOfParams, val, leg_sop, leg_dop, ocl->next);
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
		return NULL;
	}
	return ptr;
}

void populateOclist()
{
	int leg_dop[4] = {0,1,2,3};
	int leg_sop[4] = {0,1,2,3};
	opcodes = addOpcode("mov", 2, OPCODE_MOV, leg_sop, leg_dop, opcodes);
	opcodes = addOpcode("cmp", 2, OPCODE_CMP, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("add", 2, OPCODE_ADD, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("sub", 2, OPCODE_SUB, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("not", 1, OPCODE_NOT, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("clr", 1, OPCODE_CLR, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("lea", 2, OPCODE_LEA, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("inc", 1, OPCODE_INC, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("dec", 1, OPCODE_DEC, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("jmp", 1, OPCODE_JMP, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("bne", 1, OPCODE_BNE, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("red", 1, OPCODE_RED, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("prn", 1, OPCODE_PRN, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("jsr", 1, OPCODE_JSR, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("rts", 0, OPCODE_RTS, leg_sop, leg_dop,  opcodes);
	opcodes = addOpcode("stop", 0, OPCODE_STOP, leg_sop, leg_dop,  opcodes);
	
}

struct oclist *oclistAlloc(void)
{
	return (struct oclist *) malloc(sizeof(struct oclist));
}
