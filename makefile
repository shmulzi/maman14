prog: main.o reader.o label.o opcodes.o second_pass.o tran_directive.o tran_op.o foutput.o genfunc.o
	gcc -g -ansi -Wall -pedantic main.o reader.o label.o opcodes.o second_pass.o tran_directive.o tran_op.o foutput.o genfunc.o -o prog
main.o: main.c 
	gcc -c -ansi -Wall -pedantic main.c -o main.o
reader.o: reader.c
	gcc -c -ansi -Wall -pedantic reader.c -o reader.o
label.o: label.c 
	gcc -c -ansi -Wall -pedantic label.c -o label.o
tran_op.o: tran_op.c
	gcc -c -ansi -Wall -pedantic tran_op.c -o tran_op.o
tran_directive.o: tran_directive.c
	gcc -c -ansi -Wall -pedantic tran_directive.c -o tran_directive.o
opcodes.o: opcodes.c
	gcc -c -ansi -Wall -pedantic opcodes.c -o opcodes.o
second_pass.o: second_pass.c
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o
foutput.o: foutput.c
	gcc -c -ansi -Wall -pedantic foutput.c -o foutput.o
genfunc.o: genfunc.c
	gcc -c -ansi -Wall -pedantic genfunc.c -o genfunc.o