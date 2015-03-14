/*defines.h - contains pre-processor definitions that need to take effect in more that one file in the program*/

#include <stdio.h>

#define OPCODE_MOV 0
#define OPCODE_CMP 1
#define OPCODE_ADD 2
#define OPCODE_SUB 3
#define OPCODE_NOT 4
#define OPCODE_CLR 5
#define OPCODE_LEA 6
#define OPCODE_INC 7
#define OPCODE_DEC 8
#define OPCODE_JMP 9
#define OPCODE_BNE 10
#define OPCODE_RED 11
#define OPCODE_PRN 12
#define OPCODE_JSR 13
#define OPCODE_RTS 14
#define OPCODE_STOP 15

#define OP_METH_INSTANT 0
#define OP_METH_DIRECT 1
#define OP_METH_DIST 2
#define OP_METH_R_DIRECT 3

#define OP_TYPE_SRC 0
#define OP_TYPE_DEST 1
