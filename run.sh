#!/bin/bash
gcc -g main.c tran_op.c reader.c opcodes.c label.c -o prog
./prog testing
