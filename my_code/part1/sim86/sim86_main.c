#include "sim86_manager.h"

int main(u32 argc, u8 *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printf("Usage: sim86 <option> <binary_file>\n");
        return 1;
    }
    
    u8 option = ManagerParseOption(argv[1]); 
    if (option < 0)
    {
        printf("Invalid option");
        printf("Valid options: -exec, -dump -exec, -print");
        return 1;
    }

    FILE *bin = NULL;
    if (option == EXEC_BIN || option == PRINT_TO_ASM)
    {
        bin = fopen(argv[2], "rb");
    }
    else if (option == DUMP_MEM)
    {
        bin = fopen(argv[3], "rb");
    }
    if (!bin)
    {
        perror("Open file");
        return 1;
    }

    ManagerOperate(bin, option);
   
    return 0;
}