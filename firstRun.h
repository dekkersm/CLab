#ifndef MAMAN14_FIRSTRUN_H
#define MAMAN14_FIRSTRUN_H

#include "lineParsing.h"

int firstRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable, short memoryArray[]);
void parseLineFirstRun(char *currLine, SymbolNode symbolTable, short memoryArray[], short dataArray[]);

/*Guiding line parsing*/
void parseDataTypeLine(char *currLine, short dataArray[]);
void parseStringTypeLine(char *currLine, short dataArray[]);
void parseStructTypeLine(char *currLine, short dataArray[]);
void parseExternLine(char *currLine, SymbolNode symbolTable);
int parseInstructionLine(char *currLine, Command *currCmd, SymbolNode symbolTable, short memoryArray[]);

#endif
