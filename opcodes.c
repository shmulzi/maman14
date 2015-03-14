/*opcodes.c - contains the table of opcodes that were predetermined in the program requirements along with functions to access them*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "defines.h"
#include "structs.h"

#define DM_PERM 1
#define DM_NOT 0

/*self*/
struct oclist *oclist_alloc(void);

/*global parameters*/
struct oclist *opcodes;

/*permit_switch- receives 4 integers each representing if the delivery method for that param is permmited and returns a corresponding permit switch
 * returns a permit switch which is a bit shifted integer that represents which delivery method are allowed for a param*/
int permit_switch(int instant, int direct, int distance, int r_direct)
{
	int permission = instant | (direct << OP_METH_DIRECT) | (distance << OP_METH_DIST) | (r_direct << OP_METH_R_DIRECT);
	return permission;
}

/*is_method_permitted - gets an opcode an integer that represents the delivery method and an integer that represents the operand type (destination or source) and returns 1 if the delivery method is permitted or 0 if it is not
 * returns 1 if the delivery method is permitted or 0 if it is not*/
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

/*append_opcode- recieves an oclist struct object and paramters for a new one, creates it and appends it to the last node
 * returns the recieved oclist struct object with the new node appended*/
struct oclist *append_opcode(char *code, int numOfParams, int val, int leg_sop, int leg_dop, struct oclist *ocl)
{
	if(ocl == NULL)
	{
		ocl = oclist_alloc();
		ocl->code = code;
		ocl->numOfParams = numOfParams;
		ocl->val = val;
		ocl->leg_sop = leg_sop;
		ocl->leg_dop = leg_dop;
		ocl->next = NULL;
	} else {
		ocl->next = append_opcode(code, numOfParams, val, leg_sop, leg_dop, ocl->next);
	}
	return ocl;
}

/*isopcode - goes through the opcode list and returns 1 if opcode exists in list and 0 if it doesnt
 * returns 1 if opcode exists in list and 0 if it doesnt*/
int isopcode(char *code)
{
	struct oclist *ptr;
	int result = 0;
	if(opcodes != NULL){
		for(ptr = opcodes; ptr != NULL; ptr = ptr->next){
			if(strcmp(code, ptr->code) == 0){
				result = 1;
			}
		}
	} else {
		printf("opcodes.c - opcodes table has failed to populate\n");
	}
	return result;
}

/*get_opcode - goes through the opcode list and returns the opcode that corresponds with the string received, if it goes through entire list and opcode does not exist it returns null
 * returns sturct oclist opcode object if opcode exists in list and null if it doesnt*/
struct oclist *get_opcode(char *code)
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

/*populate_oclist - adds all predetrimned op codes to the opcode list as specified by project requirements*/
void populate_oclist()
{
	opcodes = append_opcode("mov", 2, OPCODE_MOV, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM), opcodes);
	opcodes = append_opcode("cmp", 2, OPCODE_CMP, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_PERM,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("add", 2, OPCODE_ADD, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("sub", 2, OPCODE_SUB, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("not", 1, OPCODE_NOT, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("clr", 1, OPCODE_CLR, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("lea", 2, OPCODE_LEA, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_NOT), permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("inc", 1, OPCODE_INC, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("dec", 1, OPCODE_DEC, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_PERM),  opcodes);
	opcodes = append_opcode("jmp", 1, OPCODE_JMP, 0, permit_switch(DM_NOT,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = append_opcode("bne", 1, OPCODE_BNE, 0, permit_switch(DM_NOT,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = append_opcode("red", 1, OPCODE_RED, 0, permit_switch(DM_NOT,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = append_opcode("prn", 1, OPCODE_PRN, 0, permit_switch(DM_PERM,DM_PERM,DM_PERM,DM_PERM),  opcodes);
	opcodes = append_opcode("jsr", 1, OPCODE_JSR, 0, permit_switch(DM_NOT,DM_PERM,DM_NOT,DM_NOT),  opcodes);
	opcodes = append_opcode("rts", 0, OPCODE_RTS, 0, 0,  opcodes);
	opcodes = append_opcode("stop", 0, OPCODE_STOP, 0, 0,  opcodes);
	
}

/*oclist_alloc - returns allocated memory for an oclist struct item*/
struct oclist *oclist_alloc(void)
{
	return (struct oclist *) malloc(sizeof(struct oclist));
}
