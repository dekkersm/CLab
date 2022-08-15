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

enum GuidingType {
    dataType = 0,
    stringType = 1,
    structType = 2,
    externalType = 3,
    entryType = 4
};

typedef struct {
    short opcode;
    int operandCount;
    AddressingMethod sourceOpLegalAddressMethods;
    AddressingMethod destOpLegalAddressMethods;
} Command;

int parseCommand(char *cmd, Command *currCmd);
void parseAssemblyLines(FILE *amFile, SymbolNode symbolTable);
void parseLine(char *currLine, SymbolNode symbolTable);

// Guiding line parsing
void parseDataTypeLine(char *currLine);
void parseStringTypeLine(char *currLine);
void parseStructTypeLine(char *currLine);
void parseExternLine(char *currLine, SymbolNode symbolTable);
int parseInstructionLine(char *currLine, Command *currCmd, SymbolNode symbolTable);
Operand* parseOperand(char *operand, SymbolNode symbolTable);
int checkAddressingMethodValidity(AddressingMethod operandAM, AddressingMethod legalAM);

#endif //MAMAN14_FIRSTRUN_H
