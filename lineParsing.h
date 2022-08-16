//
// Created by dekke on 16/08/2022.
//

#ifndef MAMAN14_LINEPARSING_H
#define MAMAN14_LINEPARSING_H

#include "dataStructs.h"

int isEmptyOrCommentLine(char *currLine);
int isSymbolDeclaration(char *firstWord);
int isGuidingLine(char const *currLine);
int classifyGuidingType(char *currLine, enum GuidingType *guidingType);
char* getExternOrEntryOperand(char *externLine);
int parseCommand(char *cmd, Command *currCmd);
Operand* parseOperand(char *operand, SymbolNode symbolTable, int isFirstRun);
int checkAddressingMethodValidity(AddressingMethod operandAM, AddressingMethod legalAM);

#endif //MAMAN14_LINEPARSING_H
