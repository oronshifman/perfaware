#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sim8086_2nd.h>

#define OP_MASK  0x00f0
#define D_MASK  0x0002
#define MOD_MASK 0xc000
#define REG_MASK 0x3800
#define RM_MASK  0x0700

#define BYTE 1
#define EIGHT_BITS 8
#define ELEVEN_BITS 11

#define MOV_REG_TO_REG 0x0080
#define MOV_IMMIDIATE_TO_REG 0x00b0
// #define 
// #define 
// #define 
// #define 

#define END_OP_OFFSET 3
#define DEST_BUF_OFFSET 4
#define SRC_BUF_OFFSET DEST_BUF_OFFSET + 4
#define NT_BUF_OFFSET SRC_BUF_OFFSET + 2

static uint8_t *op_table[256] = {0};
static uint8_t *byte_reg_table[8] = 
{
    "al", "cl", "dl", "bl", 
    "ah", "ch", "dh", "bh"
}; 

static uint8_t *word_reg_table[8] = {
    "ax", "cx", "dx", "bx",
    "sp", "bp", "si", "di"
};

static uint8_t *effective_addr_calc[8] = 
{
    "[bx + si", "[bx + di", "[bp + si", "[bp + di",
    "[si", "[di", "[bp","[bx"
};                                 

static void InitOpTable(void);
static void DecodeEAToReg(int32_t fd, uint16_t instruction);
static void DecodeRegRmEight(int32_t fd, uint16_t instruction);
static void DecodeRegRmSixteen(int32_t fd, uint16_t instruction);
static void DecodeImmediateToReg(int32_t fd, uint16_t ls_byte);
static uint8_t DecodeOp(int32_t instruction);
static void DecodeRegToReg(int32_t instruction);

void InstructionDecoder(int32_t fd)
{
    #define LOAD_8_BIT 0x4000
    #define LOAD_16_BIT 0x8000
    
    InitOpTable();

    uint16_t instruction = 0;
    while (read(fd, &instruction, BYTE * 2) > 0)
    {
        if (DecodeOp(instruction) == MOV_REG_TO_REG)
        {
            // check mod and continue acordingly
            if ((instruction & MOD_MASK) == 0) 
            {
                DecodeEAToReg(fd, instruction);
            }
            else if ((instruction & MOD_MASK) == LOAD_8_BIT)
            {
                DecodeRegRmEight(fd, instruction);
            }
            else if ((instruction & MOD_MASK) == LOAD_16_BIT)
            {
                DecodeRegRmSixteen(fd, instruction);    
            }
            else // 11, d bit is always 0
            {
                DecodeRegToReg(instruction);
            }
        }
        else // MOV_IMMIDIATE_TO_REG
        {
            DecodeImmediateToReg(fd, instruction);
        }
    }
}

static void InitOpTable(void)
{
    op_table[MOV_REG_TO_REG] = "mov";
    op_table[MOV_IMMIDIATE_TO_REG] = "mov";
}

static void DecodeEAToReg(int32_t fd, uint16_t instruction) 
{
    uint8_t w_mask = 0x1;
    uint8_t d_mask = 0x2;
    uint16_t reg_mask = 0x3800;
    uint16_t rm_mask = 0x700;
    uint16_t direct_addr = 0x600;

    if ((instruction & rm_mask) == direct_addr) 
    {
        uint16_t word_disp = 0;
        if (!(read(fd, &word_disp, BYTE * 2) > 0))
        {
            perror("DecodeImmediateToReg");
            return;
        }   
        // TODO: parse w_disp             
    }
    else
    {
        if (instruction & w_mask)
        {
            if (!(instruction & d_mask))
            {
                printf("%s], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS], 
                                    word_reg_table[(instruction & reg_mask) >> ELEVEN_BITS]);
            }
            else
            {
                printf("%s, %s]\n", word_reg_table[(instruction & reg_mask) >> ELEVEN_BITS], 
                                    effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS]);
            }

        }
        else
        {
            if (!(instruction & d_mask))
            {
                printf("%s], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS], 
                                    byte_reg_table[(instruction & reg_mask) >> ELEVEN_BITS]);
            }
            else
            {
                printf("%s, %s]\n", byte_reg_table[(instruction & reg_mask) >> ELEVEN_BITS], 
                                    effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS]);
            }
        }
    }
}

static void DecodeRegRmEight(int32_t fd, uint16_t instruction)
{
    uint8_t w_mask = 0x1;
    uint8_t d_mask = 0x2;
    uint16_t reg_mask = 0x3800;
    uint16_t rm_mask = 0x700;
    uint8_t direct_addr = 0x6;

    uint16_t byte_disp = 0;
    if (!(read(fd, &byte_disp, BYTE) > 0))
    {
        perror("DecodeImmediateToReg");
        return;
    }   
    // parse w_disp             

    if (instruction & w_mask)
    {
        if (!(instruction & d_mask))
        {
            printf("%s], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                word_reg_table[(instruction & reg_mask) >> ELEVEN_BITS]);
        }
        else
        {
            printf("%s, %s]\n", word_reg_table[(instruction & reg_mask) >> ELEVEN_BITS], 
                                effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS]);
        }

    }
    else
    {
        if (!(instruction & d_mask))
        {
            printf("%s], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                     byte_reg_table[(instruction & reg_mask) >> ELEVEN_BITS]);
        }
        else
        {
            printf("%s, %s + %d]\n", byte_reg_table[(instruction & reg_mask) >> ELEVEN_BITS], 
                                     effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                     byte_disp);
        }
    }

}

static void DecodeRegRmSixteen(int32_t fd, uint16_t instruction)
{
    uint8_t w_mask = 0x8;
    uint8_t d_mask = 0x2;
    uint8_t reg_mask = 0x38;
    uint8_t rm_mask = 0x7;

    uint16_t word_disp = 0;
    if (!(read(fd, &word_disp, BYTE * 2) > 0))
    {
        perror("DecodeImmediateToReg");
        return;
    } 

    if (instruction & w_mask)
    {
        printf("%s, %s + %d]\n", word_reg_table[(instruction & reg_mask) >> ELEVEN_BITS], 
                                 effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                 word_disp);  
    }   
    else
    {
        printf("%s, %s + %d]\n", byte_reg_table[(instruction & reg_mask) >> ELEVEN_BITS], 
                                 effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                 word_disp);  
    }
    
}

static void DecodeImmediateToReg(int32_t fd, uint16_t instruction)
{
    uint8_t w_mask = 0x8;
    uint16_t reg_mask = 0x7;
    uint16_t one_byte_immidiate = 0Xff00;

    if (instruction & w_mask)
    {
        // load 8 more bits
        uint16_t w_immidiate = 0;
        if (!(read(fd, &w_immidiate, BYTE) > 0))
        {
            perror("DecodeImmediateToReg");
            return;
        }
        // parss the extra byte
        printf("%s, %d\n", word_reg_table[(instruction & reg_mask)], 
                           w_immidiate << EIGHT_BITS | ((instruction & 0xff00) >> EIGHT_BITS));
    }
    else
    {
        printf("%s, %d\n", byte_reg_table[(instruction & reg_mask)], 
                           (instruction & one_byte_immidiate) >> EIGHT_BITS);    
    }

}

static uint8_t DecodeOp(int32_t instruction)
{
    uint8_t operation = (instruction & OP_MASK);
    printf("%s ", op_table[operation]);

    return operation;
}

static void DecodeRegToReg(int32_t instruction)
{
    uint8_t w_mask = 0x0001;

    if (instruction & w_mask)
    {
        printf("%s, %s\n", word_reg_table[(instruction & RM_MASK) >> EIGHT_BITS], 
                           word_reg_table[(instruction & REG_MASK) >> ELEVEN_BITS]);
    }
    else
    {
        printf("%s, %s\n", byte_reg_table[(instruction & RM_MASK) >> EIGHT_BITS], 
                           byte_reg_table[(instruction & REG_MASK) >> ELEVEN_BITS]);
    }
}