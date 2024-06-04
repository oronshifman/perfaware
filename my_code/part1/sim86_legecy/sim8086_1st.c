#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define OP_MASK  0x00fc
#define D_MASK  0x0002
#define W_MASK  0x0001
#define MOD_MASK 0xc000
#define REG_MASK 0x3800
#define RM_MASK  0x0700

#define INSTRUCTION_SIZE 2

#define MOV 0x0088

#define DEST_BUF_OFFSET 4
#define SRC_BUF_OFFSET DEST_BUF_OFFSET + 4
#define NT_BUF_OFFSET SRC_BUF_OFFSET + 2

// static uint8_t op_table[256][3] = {0};
// static uint8_t *dw_table[4] = {};
// static uint8_t *mod_table[4] = {0};
// NOTE: if w is set the '_l' reginsters become '_x'
static uint8_t *byte_reg_table[8] = {"al", "cl", "dl", "bl", 
                                     "ah", "ch", "dh", "bh"}; 
static uint8_t *word_reg_table[8] = {"ax", "cx", "dx", "bx",
                                     "sp", "bp", "si", "di"};
// static uint8_t *rm_table[7] = {0};

void InstructionDecoder(uint16_t instruction, uint8_t *buffer);

int32_t main(int32_t argc, uint8_t *argv[])
{
    if (argc < 2)
    {
        printf("Usage: sim8086 binary_file\n");
        return 1;
    }

    int32_t fd = 0;
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        perror("Open file");
        return 1;
    }

    uint8_t output_buf[16] = {'\0'};
    uint16_t instruction = 0;
    printf("bits 16\n");
    while (read(fd, &instruction, INSTRUCTION_SIZE) > 0)
    {
        InstructionDecoder(instruction, output_buf);
        printf("%s", output_buf);
    }


    return 0;
}

void InstructionDecoder(uint16_t instruction, uint8_t *buffer)
{
    (instruction & OP_MASK) == MOV ? memcpy(buffer, "mov ", 4) : 0;

    if (((instruction & MOD_MASK) >> 14) == 0x3)
    {
        /*
        REG is dest:
        if (W)
        {
            get the value from REG
            write coresponding string from word_reg_table to buffer
            get the value from RM
            write coresponding string from word_reg_table to buffer
        }
        else
        {
            get the value from REG
            write coresponding string from byte_reg_table to buffer
            get the value from RM
            write coresponding string from byte_reg_table to buffer
        }
        */
        if (instruction & W_MASK)
        {
            memcpy(buffer + DEST_BUF_OFFSET, word_reg_table[(instruction & RM_MASK) >> 8], 2);
            memcpy(buffer + SRC_BUF_OFFSET - 2, ", ", 2);
            memcpy(buffer + SRC_BUF_OFFSET, word_reg_table[(instruction & REG_MASK) >> 11], 2);
            *(buffer + NT_BUF_OFFSET) = '\n';
        }
        else
        {
            memcpy(buffer + DEST_BUF_OFFSET, byte_reg_table[(instruction & RM_MASK) >> 8], 2);
            memcpy(buffer + SRC_BUF_OFFSET - 2, ", ", 2);
            memcpy(buffer + SRC_BUF_OFFSET, byte_reg_table[(instruction & REG_MASK) >> 11], 2);
            *(buffer + NT_BUF_OFFSET) = '\n';
        }
    }
    else
    {
        /*
        REG is src:
        if (W)
        {
            get the value from RM
            write coresponding string from word_reg_table to buffer
            get the value from REG
            write coresponding string from word_reg_table to buffer
        }
        else
        {
            get the value from RM
            write coresponding string from byte_reg_table to buffer
            get the value from REG
            write coresponding string from byte_reg_table to buffer
        }
        */
        if (instruction & W_MASK)
        {
            memcpy(buffer + DEST_BUF_OFFSET, word_reg_table[(instruction & REG_MASK) >> 11], 2);
            memcpy(buffer + SRC_BUF_OFFSET - 2, ", ", 2);
            memcpy(buffer + SRC_BUF_OFFSET, word_reg_table[(instruction & RM_MASK) >> 8], 2);
            *(buffer + NT_BUF_OFFSET) = '\n';
        }
        else
        {
            memcpy(buffer + DEST_BUF_OFFSET, byte_reg_table[(instruction & REG_MASK) >> 11], 2);
            memcpy(buffer + SRC_BUF_OFFSET - 2, ", ", 2);
            memcpy(buffer + SRC_BUF_OFFSET, byte_reg_table[(instruction & RM_MASK) >> 8], 2);
            *(buffer + NT_BUF_OFFSET) = '\n';
        }
    }
}

