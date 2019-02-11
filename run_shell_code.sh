#!/bin/bash

gcc s_proc.c -o s_proc
nasm -o shell_code shell_code.asm
./s_proc shell_code
gcc exec_shell_code.c -o exec_shell_code
./exec_shell_code
rm -rf shell_code exec_shell_code* s_proc
