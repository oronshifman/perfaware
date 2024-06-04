#!/bin/bash

# with decoder
gcc -I. sim86_instruction.c sim86_manager.c sim86_printer.c sim86_executer.c sim86_decoder.c sim86_memory.c sim86_main.c -o sim86.out -g

# without decoder
# gcc -I. sim86_manager.c sim86_printer.c sim86_executer.c sim86_memory.c sim86_main.c -o sim86 -g
