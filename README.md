By: Oron Shifman

This project is following the course "Performance-Aware Programming" by Casey Muratori.

The Program can be run by cloning the repo, building the binary with the build_sim86.sh and then running the binary as follows:
sim86.out [option] [binary_to_operat_on]

[option]: Is eather -print or -exec.
  -print: will print a disassembly of [binary_to_operat_on]
  -exec: will execut the instruction in [binary_to_operat_on] on the memory module and print out info about the execution prosses.

The program supports dissassembly operations that appear in the listings in decoding_listing_asm directory, and execution of operations that appear in execution_listing_asm.
