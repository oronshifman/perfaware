#!/bin/bash
mkdir -p ../build
pushd ../build

# with decoder
gcc ../sim86/*.c -o sim86.out -g -I/home/oron/git/perfaware/my_code/part1/sim86

# without decoder
# gcc -I/home/oron/git/perfaware/my_code/part1/sim86 sim86_manager.c sim86_printer.c sim86_executer.c sim86_memory.c sim86_main.c -o sim86 -g

popd
