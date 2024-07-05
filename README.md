---
# Project Name: simple_8086_simulator
### By: Oron Shifman

This project follows part1 of the course "Performance-Aware Programming" by Casey Muratori.
This is a simplefied simulation of the 8086 architecture, build for learning purposes.

## Description

This program, `sim86`, provides functionality for disassembling and executing x86 assembly instructions.

## Usage

1. **Clone the repository:**
   ```
   git clone https://github.com/oronshifman/perfaware.git
   cd perfawer
   ```

2. **Build the binary:**
   ```
   ./build_sim86.sh
   ```

3. **Run the program:**
   ```
   ./sim86.out [option] [binary_to_operate_on]
   ```

   - `[option]` can be either `-print` or `-exec`:
     - `-print`: Print a disassembly of `[binary_to_operate_on]`.
     - `-exec`: Execute the instructions in `[binary_to_operate_on]` on the memory module and print information about the execution process.

## Supported Operations

The program supports:
- Disassembly operations found in the listings in the `decoding_listing_asm` directory.
- Execution of operations found in the `execution_listing_asm` directory.

## Examples

### Printing Disassembly

To print the disassembly of a binary file named `example_binary`:
```
./sim86.out -print example_binary
```

### Executing Instructions

To execute the instructions of `example_binary` and print execution information:
```
./sim86.out -exec example_binary
```

---
