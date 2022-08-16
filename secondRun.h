//
// Created by dekke on 16/08/2022.
//

#ifndef MAMAN14_SECONDRUN_H
#define MAMAN14_SECONDRUN_H

#include "lineParsing.h"

void secondRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable, short instructionsArray[], ExternNode externList);
void parseEntryLine(char *currLine, SymbolNode symbolTable);
void parseLineSecondRun(char *currLine, SymbolNode symbolTable, short memoryArray[], ExternNode externList);
short parseInstructionLineOperands(char *currLine, Command *currCmd, SymbolNode symbolTable, short memoryArray[], ExternNode externList);

#endif //MAMAN14_SECONDRUN_H
