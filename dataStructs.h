#ifndef MAMAN14_DATASTRUCTS_H
#define MAMAN14_DATASTRUCTS_H

#include "utils.h"
#include "globals.h"

typedef enum {
    num = 0,
    symbol = 1,
    struc = 2,
    reg = 3,
} operandType;

typedef enum {
    external = 0,
    data = 1,
    code = 2,
    entry = 3
} SymbolType;

typedef enum {
    absolute = 0,
    relocatable = 2,
    ext = 1,
} ARE;

enum GuidingType {
    dataType = 0,
    stringType = 1,
    structType = 2,
    externalType = 3,
    entryType = 4
};

typedef struct {
    int immediate : 1;
    int direct : 1;
    int structIndex : 1;
    int directRegister : 1;
} AddressingMethod;


typedef struct {
    short opcode;
    int operandCount;
    AddressingMethod sourceOpLegalAddressMethods;
    AddressingMethod destOpLegalAddressMethods;
} Command;

typedef struct {
    operandType type;
    AddressingMethod addressingMethod;
    ARE are;
    short value;
    short fieldValue; /* optional*/
    char symbolName[SYMBOL_MAX_CHAR_LENGTH]; /*optional*/
} Operand;

struct SymbolTable
{
    char name[SYMBOL_MAX_CHAR_LENGTH];
    short value;
    SymbolType type; /*external/entry/Data/Code*/
    int isRelocatable;
    struct SymbolTable* next;
};

struct ExternTable
{
    char name[SYMBOL_MAX_CHAR_LENGTH];
    short value;
    struct ExternTable* next;
};

typedef struct SymbolTable *SymbolNode; /*Define node as pointer of data type struct LinkedList*/
typedef struct ExternTable *ExternNode;
SymbolNode createSymbolNode();
SymbolNode addSymbolNode(SymbolNode head, char *name, short value, SymbolType type, int isRelocatable);
SymbolNode getSymbolByName(SymbolNode head, char *name);

ExternNode createExternNode();
ExternNode addExternNode(ExternNode head, char *name, short value);

#endif
