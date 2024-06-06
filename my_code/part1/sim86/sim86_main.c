#include "sim86_manager.h"

int32_t main(int32_t argc, uint8_t *argv[])
{
    if (argc < 3)
    {
        printf("Usage: sim86 <option> <binary_file>\n");
        return 1;
    }
    
    uint8_t option = ParseOption(argv[1]); 
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

    DecodeBin(bin, option);
   
    return 0;
}