#!/bin/bash
mkdir -p ../build
pushd ../build

# full build
gcc -g ../sim86/*.c -o sim86.out -I/home/oron/git/perfaware/my_code/part1/sim86

# without decoder
# gcc ../sim86/sim86_manager.c ../sim86/sim86_printer.c ../sim86/sim86_executer.c ../sim86/sim86_memory.c ../sim86/sim86_main.c -o sim86_no_decoder.out -g -I/home/oron/git/perfaware/my_code/part1/sim86

# test memory
# gcc ../sim86/sim86_memory.c ../test/sim86_memory_test.c -o sim86_tets_memory.out -g -I/home/oron/git/perfaware/my_code/part1/sim86

popd
