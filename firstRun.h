//
// Created by dekke on 08/08/2022.
//

#ifndef MAMAN14_FIRSTRUN_H
#define MAMAN14_FIRSTRUN_H

#include "lineParsing.h"

void firstRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable);
void parseLineFirstRun(char *currLine, SymbolNode symbolTable);

// Guiding line parsing
void parseDataTypeLine(char *currLine);
void parseStringTypeLine(char *currLine);
void parseStructTypeLine(char *currLine);
void parseExternLine(char *currLine, SymbolNode symbolTable);
int parseInstructionLine(char *currLine, Command *currCmd, SymbolNode symbolTable);
Operand* parseOperand(char *operand, SymbolNode symbolTable);
int checkAddressingMethodValidity(AddressingMethod operandAM, AddressingMethod legalAM);

#endif //MAMAN14_FIRSTRUN_H
