//
// Created by dekke on 08/08/2022.
//

#ifndef MAMAN14_FIRSTRUN_H
#define MAMAN14_FIRSTRUN_H

#include "utils.h"
#include "globals.h"
#include "dataStructs.h"

#define MEMORY_WORD_BIT_SIZE 10
#define MEMORY_ARRAY_WORD_SIZE 256

typedef struct {
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int structIndex : 1;
    unsigned int directRegister : 1;
} AddressingMethod;

enum GuidingType {
    dataType = 0,
    stringType = 1,
    structType = 2,
    external = 3,
    entry = 4
};

typedef struct {
    int opcode;
    int operandCount;
    AddressingMethod sourceOpLegalAddressMethods;
    AddressingMethod destOpLegalAddressMethods;
}Command;

int parseCommand(char *cmd, Command *currCmd);
void parseAssemblyLines(FILE *amFile, SymbolNode symbolTable);
void parseLine(char *currLine, SymbolNode symbolTable);

#endif //MAMAN14_FIRSTRUN_H
