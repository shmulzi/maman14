#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "defines.h"
#include "structs.h"

#define DM_PERM 1
#define DM_NOT 0

struct oclist *oclistAlloc(void);

struct oclist *opcodes;

int permit_switch(int instant, int direct, int distance, int r_direct)
{
	int permission = instant | (direct << OP_METH_DIRECT) | (distance << OP_METH_DIST) | (r_direct << OP_METH_R_DIRECT);
	return permission;
}

int is_method_permitted(struct oclist *ocitem, int meth, int op_type)
{
	int mask = 1 << meth;
	int result = 1;
	if(op_type == OP_TYPE_DEST){
		result = ocitem->leg_dop & mask;
	} else if (op_type == OP_TYPE_SRC) {
		result = ocitem->leg_sop & mask;
	}
	return !result;
}

struct oclist *addOpcode(char *code, int numOfParams, int val, int leg_sop, int leg_dop, struct oclist *ocl)
{
	if(ocl == NULL)
	{
		ocl = oclistAlloc();
		ocl->code = code;
		ocl->numOfParams = numOfParams;
		ocl->val = val;
		ocl->leg_sop = leg_sop;
		ocl->leg_dop = leg_dop;
		ocl->next = NULL;
	} else {
		ocl->next = addOpcode(code, numOfParams, val, leg_sop, leg_dop, ocl->next);
	}
	return ocl;
}

void printOpcodeList(){
	struct oclist *ptr;
	ptr = opcodes;
	printf("\nPrinting OpCode List: \n");
	for(; ptr != NULL; ptr = ptr->next)
	{
		printf("ncode: %s numofparams: %d val: %d allowed d_op: %d allowed s_op: %d\n", ptr->code, ptr->numOfParams, ptr->val, ptr->leg_dop, ptr->leg_sop);
	}
}

int isopcode(char *code)
{
	struct oclist *ptr;
	int result = -1;
	if(opcodes != NULL){
		for(ptr = opcodes; ptr != NULL; ptr = ptr->next){
			if(strcmp(code, ptr->code) == 0){
				result = 0;
			}
		}
	} else {
		printf("opcodes.c - opcodes table has failed to populate\n");
	}
	return result;
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
	opcodes = addOpcode("mov", 2, OPCODE_MOV, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM), opcodes);
	opcodes = addOpcode("cmp", 2, OPCODE_CMP, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_PERM,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("add", 2, OPCODE_ADD, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("sub", 2, OPCODE_SUB, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("not", 1, OPCODE_NOT, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("clr", 1, OPCODE_CLR, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("lea", 2, OPCODE_LEA, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_NOT), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("inc", 1, OPCODE_INC, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("dec", 1, OPCODE_DEC, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = addOpcode("jmp", 1, OPCODE_JMP, 0, permit_switch(DM_NOT,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = addOpcode("bne", 1, OPCODE_BNE, 0, permit_switch(DM_NOT,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = addOpcode("red", 1, OPCODE_RED, 0, permit_switch(DM_NOT,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = addOpcode("prn", 1, OPCODE_PRN, 0, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = addOpcode("jsr", 1, OPCODE_JSR, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_NOT),  opcodes);
	opcodes = addOpcode("rts", 0, OPCODE_RTS, 0, 0,  opcodes);
	opcodes = addOpcode("stop", 0, OPCODE_STOP, 0, 0,  opcodes);
	
}

struct oclist *oclistAlloc(void)
{
	return (struct oclist *) malloc(sizeof(struct oclist));
}
