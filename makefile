assembler: main.c opcodes.c reader.c foutput.c label.c tran_directive.c tran_op.c
	gcc -g -ansi -Wall -pedantic main.c opcodes.c reader.c foutput.c label.c tran_directive.c tran_op.c -o assembler