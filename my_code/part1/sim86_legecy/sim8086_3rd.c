#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "sim8086_public.h"

enum mask 
{
    OP_MASK = 0xfc,
    D_MASK = 0x2,
    MOD_MASK = 0xc000,
    REG_MASK = 0x3800,
    RM_MASK =  0x700
};

enum offsets
{
    END_OP_OFFSET = 3,
    DEST_BUF_OFFSET = 4,
    SRC_BUF_OFFSET = DEST_BUF_OFFSET  + 4,
    NT_BUF_OFFSET = SRC_BUF_OFFSET  + 2
};

enum op_type
{
    JMP,
    RM_TO_FROM_REG,
    IMMIDIATE_TO_REG,
    IMMIDIATE_TO_RM,
    IMMIDIATE_TO_ACC
};

#define BYTE 1
#define EIGHT_BITS 8
#define ELEVEN_BITS 11

static uint8_t *op_table[256] = {0};
static uint8_t *reg_table[2][8] = 
{
    // byte (0)
    {"al", "cl", "dl", "bl", 
     "ah", "ch", "dh", "bh"},
    // word (1)
    {"ax", "cx", "dx", "bx",
    "sp", "bp", "si", "di"}
};

static uint8_t *effective_addr_calc[8] = 
{
    "[bx + si", "[bx + di", "[bp + si", "[bp + di",
    "[si", "[di", "[bp","[bx"
};                                 

// TODO: implement instruction struct and use in all functions
// typedef struct instruction
// {
//     uint8_t operations;
//     uint8_t w;
//     uint8_t d;
//     uint8_t mod;
//     uint8_t reg;
//     uint8_t rm;
//     uint16_t disp;
//     uint16_t data;
//     int8_t jmp_offset;
// } instruction_t;

static void InitOpTable(void);
static void DecodeEAToReg(int32_t fd, uint16_t instruction);
static void DecodeRegRm(int32_t fd, uint16_t instruction);
static void DecodeImmediateToReg(int32_t fd, uint16_t ls_byte);
static uint8_t DecodeOp(int32_t instruction);
static uint8_t GetOpType(int32_t instruction);
static void DecodeImmediateToRm(int32_t fd, uint16_t instruction);
static void DecodeImmediateToAcc(int32_t fd, uint16_t instruction);
static void DecodeMovImmidiateToRM(int32_t fd, uint16_t instruction, uint16_t disp);
static uint16_t ExtraBytesToRead(uint16_t instruction);
static void DecodeJmp(uint16_t instruction);

// TODO: pull out to a diffrent file
static void InitOpTable(void)
{
    #define MOV_RM_TO_FROM_REG 0b10001000
    #define MOV_IMMIDIATE_TO_RM 0b11000100
    #define MOV_IMMIDIATE_TO_REG 0b10110000
    #define MOV_IMMIDIATE_TO_REG_W 0b10111000
    #define MOV_IMMIDIATE_TO_REG_REG 0b10110100
    #define MOV_IMMIDIATE_TO_REG_W_REG 0b10111100

    #define ADD_SUB_CMP_IMMIDIAT_TO_RM 0b10000000 // determen the kind of op to be done

    #define ADD_RM_TO_FROM_REG 0b00000000
    #define ADD_IMMIDIAT_TO_RM 0b000 // reg field
    #define ADD_IMMIDIATE_TO_ACC 0b00000100

    #define SUB_RM_TO_FROM_REG 0b00101000
    #define SUB_IMMIDIAT_TO_RM 0b101 // reg field
    #define SUB_IMMIDIATE_TO_ACC 0b00101100

    #define CMP_RM_TO_FROM_REG 0b00111000
    #define CMP_IMMIDIAT_TO_RM 0b111 // reg field
    #define CMP_IMMIDIATE_TO_ACC 0b00111100

    #define JE 0b01110100
    #define JL 0b01111100
    #define JLE 0b01111110
    #define JB 0b01110010
    #define JBE 0b01110110
    #define JP 0b01111010
    #define JO 0b01110000
    #define JS 0b01111000
    #define JNE 0b01110101
    #define JNL 0b01111101
    #define JG 0b01111111
    #define JNB 0b01110011
    #define JA 0b01110111
    #define JNP 0b01111011
    #define JNO 0b01110001
    #define JNS 0b01111001
    #define LOOP 0b11100010
    #define LOOPZ 0b11100001
    #define LOOPNZ 0b11100000
    #define JCXZ 0b11100011
    
    op_table[MOV_RM_TO_FROM_REG] = "mov";
    op_table[MOV_IMMIDIATE_TO_RM] = "mov";
    op_table[MOV_IMMIDIATE_TO_REG] = "mov";
    op_table[MOV_IMMIDIATE_TO_REG_W] = "mov";
    op_table[MOV_IMMIDIATE_TO_REG_REG] = "mov";
    op_table[MOV_IMMIDIATE_TO_REG_W_REG] = "mov";

    op_table[ADD_RM_TO_FROM_REG] = "add";
    op_table[ADD_IMMIDIAT_TO_RM] = "add";
    op_table[ADD_IMMIDIATE_TO_ACC] = "add";

    op_table[SUB_RM_TO_FROM_REG] = "sub";
    op_table[SUB_IMMIDIAT_TO_RM] = "sub";
    op_table[SUB_IMMIDIATE_TO_ACC] = "sub";

    op_table[CMP_RM_TO_FROM_REG] = "cmp";
    op_table[CMP_IMMIDIAT_TO_RM] = "cmp";
    op_table[CMP_IMMIDIATE_TO_ACC] = "cmp";

    op_table[JE] = "je";
    op_table[JL] = "jl";
    op_table[JLE] = "jle";
    op_table[JB] = "jb";
    op_table[JBE] = "jbe";
    op_table[JP] = "jp";
    op_table[JO] = "jo";
    op_table[JS] = "js";
    op_table[JNE] = "jne";
    op_table[JNL] = "jnl";
    op_table[JG] = "jg";
    op_table[JNB] = "jnb";
    op_table[JA] = "ja";
    op_table[JNP] = "jnp";
    op_table[JNO] = "jno";
    op_table[JNS] = "jns";
    op_table[LOOP] = "loop";
    op_table[LOOPZ] = "loopz";
    op_table[LOOPNZ] = "loopnz";
    op_table[JCXZ] = "jcxz";
}

void InstructionDecoder(int32_t fd)
{
    assert(fd);

    // #define MOD_REG_TO_REG 0xc000
    
    InitOpTable();

    uint16_t instruction = 0;
    while (read(fd, &instruction, BYTE * 2) > 0)
    {
        uint8_t op_type = GetOpType(instruction);
// printf("%d\n", op_type); // NOTE: print for debugging++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (JMP == op_type) 
        {
            // TODO: handle jumps
            DecodeJmp(instruction);
        }
        // check MOD and go the aproprate route
        // hanle operation at the end of the docoding
        else if (RM_TO_FROM_REG == op_type)
        {
            // TODO: fix handling for effective address
            DecodeRegRm(fd, instruction);
        }
        else if (IMMIDIATE_TO_RM == op_type)
        {
            DecodeImmediateToRm(fd, instruction);
        }
        else if (IMMIDIATE_TO_REG == op_type) 
        {
            // TODO: trans form to immidiate to rm. handle the mov added from listing 40 to 39
            DecodeImmediateToReg(fd, instruction);
        }
        else if (IMMIDIATE_TO_ACC == op_type)
        {
            DecodeImmediateToAcc(fd, instruction);
        }
    }
}

static void DecodeJmp(uint16_t instruction)
{
    assert(instruction);

    int8_t jmp_offset = ((instruction & 0xff00) >> EIGHT_BITS);

    printf("%s ", op_table[(uint8_t)instruction]);
    printf("%d\n", jmp_offset);
}

static void DecodeImmediateToAcc(int32_t fd, uint16_t instruction)
{
    assert(instruction);

    uint8_t w_mask = 0x1;
    uint16_t one_byte_immidiate = 0Xff00;

    if (instruction & w_mask)
    {
        // load 8 more bits
        uint16_t w_immidiate = 0;
        if (!(read(fd, &w_immidiate, BYTE) > 0))
        {
            perror("DecodeImmediateToAcc");
            return;
        }
        printf("%s ", op_table[DecodeOp(instruction)]);
        printf("ax, %d\n", w_immidiate << EIGHT_BITS | ((instruction & 0xff00) >> EIGHT_BITS));
    }
    else
    {
        printf("%s ", op_table[DecodeOp(instruction)]);
        printf("al, %d\n", (instruction & one_byte_immidiate) >> EIGHT_BITS);    
    }    
}

static void DecodeEAToReg(int32_t fd, uint16_t instruction) 
{
    assert(instruction);

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
            perror("DecodeEAToReg");
            return;
        }   
        // TODO: parse w_disp??? 
    }
    else
    {
        if (!(instruction & d_mask))
        {
            printf("%s ", op_table[DecodeOp(instruction)]);
            printf("%s], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS], 
                                reg_table[instruction & w_mask][(instruction & reg_mask) >> ELEVEN_BITS]);
        }
        else
        {
            printf("%s ", op_table[DecodeOp(instruction)]);
            printf("%s, %s]\n", reg_table[instruction & w_mask][(instruction & reg_mask) >> ELEVEN_BITS],
                                effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS]);
        }
    }
}
// TODO: trim this function and maybe combine it with DecodeEAToReg
static void DecodeRegRm(int32_t fd, uint16_t instruction)
{
    assert(instruction);

    uint8_t w_mask = 0x1;
    uint8_t d_mask = 0x2;
    uint16_t reg_mask = 0x3800;
    uint16_t rm_mask = 0x700;
    uint8_t direct_addr = 0x6;

    uint8_t mod = (instruction & MOD_MASK) >> 14;

    if (!mod)
    {
        DecodeEAToReg(fd, instruction);
        return;
    }

    if (mod == 0b01 || mod == 0b10)
    {
        uint16_t disp = 0;
        if (!(read(fd, &disp, BYTE * mod) > 0))
        {
            perror("DecodeRegRm");
            return;
        } 
        // TODO: need to handl diret_addres when mod == 0
        if (!(instruction & d_mask))
        {
            printf("%s ", op_table[DecodeOp(instruction)]);
            if (disp)
            {
                printf("%s + %d], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS], disp,
                                         reg_table[instruction & w_mask][(instruction & reg_mask) >> ELEVEN_BITS]);
            }
            else
            {
                printf("%s], %s\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                    reg_table[instruction & w_mask][(instruction & reg_mask) >> ELEVEN_BITS]);
            }
        }
        else
        {
            // NOTE: maybe no need for the commented out code
            printf("%s ", op_table[DecodeOp(instruction)]);
            if (disp)
            {
                printf("%s, %s + %d]\n", reg_table[instruction & w_mask][(instruction & reg_mask) >> ELEVEN_BITS],
                                         effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                         disp);
            }
            else
            {
                printf("%s, %s]\n", reg_table[instruction & w_mask][(instruction & reg_mask) >> ELEVEN_BITS],
                                effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS]);
            }
        }
    }
    else
    {
        printf("%s ", op_table[DecodeOp(instruction)]);
        printf("%s, %s\n", reg_table[instruction & w_mask][(instruction & RM_MASK) >> EIGHT_BITS],
                           reg_table[instruction & w_mask][(instruction & REG_MASK) >> ELEVEN_BITS]);
    }

}

static void DecodeImmediateToReg(int32_t fd, uint16_t instruction)
{
    assert(instruction);

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
        printf("%s ", op_table[DecodeOp(instruction)]);
        printf("%s, %d\n", reg_table[(instruction & w_mask) >> 3][(instruction & reg_mask)],
                           w_immidiate << EIGHT_BITS | ((instruction & 0xff00) >> EIGHT_BITS));
    }
    else
    {
        printf("%s ", op_table[DecodeOp(instruction)]);
        printf("%s, %d\n", reg_table[(instruction & w_mask) >> 3][(instruction & reg_mask)],
                           (instruction & one_byte_immidiate) >> EIGHT_BITS);    
    }
}


static void DecodeImmediateToRm(int32_t fd, uint16_t instruction)
{
    assert(instruction);

    uint8_t w_mask = 0x1;
    uint8_t s_mask = 0x2;
    uint16_t reg_mask = 0x3800;
    uint16_t rm_mask = 0x700;

    // read mod
    uint8_t mod = (instruction & MOD_MASK) >> 14;
    
    uint16_t disp = 0;
    if (mod == 0b01 || mod == 0b10)
    {
        // read disp according to mod
        if (!(read(fd, &disp, BYTE * mod) > 0))
        {
            perror("DecodeImmediateToRM: Failed reading when mod 01 or 10");
            return;
        } 
    }

    // if op == MOV_IMMIDIATE_TO_RM
    if (MOV_IMMIDIATE_TO_RM == (instruction & OP_MASK))
    {
        DecodeMovImmidiateToRM(fd, instruction, disp);
        return;
    }

    // read data
    uint8_t extra_data = ExtraBytesToRead(instruction);
    uint16_t data = 0;
    if (extra_data)
    {
        if (!(read(fd, &data, BYTE * extra_data) > 0))
        {
            perror("DecodeImmediateToRM: Failed reading when mod 00");
            return;
        }
    }

    printf("%s ", op_table[(instruction & REG_MASK) >> ELEVEN_BITS]);
    if (mod == 0b11)
    {
        printf("%s, %d\n", reg_table[instruction & w_mask][(instruction & RM_MASK) >> EIGHT_BITS], data);
    }
    else
    {
        if (!mod)
        {
            printf("%s], %d\n", effective_addr_calc[(instruction & RM_MASK) >> EIGHT_BITS], data);
        }
        else
        {
            printf("%s + %d], %d\n", effective_addr_calc[(instruction & RM_MASK) >> EIGHT_BITS],
                                     disp, data);
        }
    }
}

static uint16_t ExtraBytesToRead(uint16_t instruction)
{
    uint16_t extra_bytes = 0;
    switch (instruction & 0b11) 
    {
        case 0b11:
        case 0b00:
        {
            extra_bytes = 1;
        } break;

        case 0b01:
        {
            extra_bytes = 2;
        } break;

        case 0b10:
        {
            extra_bytes = 0;
        } break;
    }

    return extra_bytes;
}

// NOTE: called from wihtin DecodeImmidiateToRM
static void DecodeMovImmidiateToRM(int32_t fd, uint16_t instruction, uint16_t disp)
{
    uint8_t w_mask = 0x1;
    uint8_t s_mask = 0x2;
    uint16_t rm_mask = 0x700;

    uint16_t data = 0;
    if (!(read(fd, &data, BYTE + (instruction & w_mask)) > 0))
    {
        perror("DecodeMovImmidiateToRM");
        return;
    }

    switch ((instruction & MOD_MASK) >> 14)
    {
        case 0b11:
        {
            // TODO: finish testing and implementation
            // printf("%s ", op_table[DecodeOp(instruction)]);
            // printf("%s, %d\n", reg_table[(instruction & w_mask) >> 3][(instruction & rm_mask)],
            //                    (instruction & data) >> EIGHT_BITS); 
        } break;

        case 0b01:
        {
            // TODO: finish testing and implementation
            // printf("%s ", op_table[DecodeOp(instruction)]);
            // printf("%s, %d\n", reg_table[(instruction & w_mask) >> 3][(instruction & rm_mask)],
            //                    (instruction & data) >> EIGHT_BITS); 
        } break;

        case 0b10:
        {
            printf("%s ", op_table[DecodeOp(instruction)]);
            printf("%s + %d], %d\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                     disp, data);
        } break;

        case 0b00:
        {
            printf("%s ", op_table[DecodeOp(instruction)]);
            printf("%s], %d\n", effective_addr_calc[(instruction & rm_mask) >> EIGHT_BITS],
                                data << (EIGHT_BITS * (instruction & w_mask)) | ((data & 0xff00) >> EIGHT_BITS));
        } break;
    }
}

static uint8_t DecodeOp(int32_t instruction)
{
    assert(instruction);

    uint8_t operation = (instruction & OP_MASK);

    return operation;
}

static uint8_t GetOpType(int32_t instruction)
{
    assert(instruction);

    uint8_t op_type = 0;
    switch (instruction & OP_MASK)
    {
        case MOV_RM_TO_FROM_REG:
        case ADD_RM_TO_FROM_REG:
        case SUB_RM_TO_FROM_REG:
        case CMP_RM_TO_FROM_REG:
        {
            op_type = RM_TO_FROM_REG;
        } break;

        case MOV_IMMIDIATE_TO_RM:
        case ADD_SUB_CMP_IMMIDIAT_TO_RM:
        {
            op_type = IMMIDIATE_TO_RM;
        } break;
        
        case MOV_IMMIDIATE_TO_REG:
        case MOV_IMMIDIATE_TO_REG_W:
        case MOV_IMMIDIATE_TO_REG_REG:
        case MOV_IMMIDIATE_TO_REG_W_REG:
        {
            op_type = IMMIDIATE_TO_REG;
        } break;

        case ADD_IMMIDIATE_TO_ACC:
        case SUB_IMMIDIATE_TO_ACC:
        case CMP_IMMIDIATE_TO_ACC:
        {
            op_type = IMMIDIATE_TO_ACC;
        } break;
        
        default:
        {
            op_type = JMP;
        } break;
    }

    return op_type;
}

