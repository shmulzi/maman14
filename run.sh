#!/bin/bash
gcc -g -ansi -Wall main.c tran_op.c tran_directive.c reader.c opcodes.c label.c -o prog
./prog testing
