#include "sim86_manager.h"

int main(u32 argc, u8 *argv[])
{
    if (argc < 3)
    {
        printf("Usage: sim86 <option> <binary_file>\n");
        return 1;
    }
    
    u8 option = ManagerParseOption(argv[1]); 
    if (option < 0)
    {
        printf("Invalid option");
        printf("Valid options: -exec, -print");
        return 1;
    }

    FILE *bin = fopen(argv[2], "rb");
    if (!bin)
    {
        perror("Open file");
        return 1;
    }

    ManagerOperate(bin, option);
   
    return 0;
}