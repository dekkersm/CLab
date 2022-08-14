//
// Created by dekke on 14/08/2022.
//

#ifndef MAMAN14_DATASTRUCTS_H
#define MAMAN14_DATASTRUCTS_H

#include "utils.h"
#include "globals.h"
#include "string.h"

enum SymbolType {
    external = 0,
    data = 1,
    code = 2,
    entry = 3
};

struct SymbolTable
{
    char name[SYMBOL_MAX_CHAR_LENGTH];
    int value;
    enum SymbolType type; // external/entry/Data/Code
    int isRelocatable;
    struct SymbolTable* next;
};

typedef struct SymbolTable *SymbolNode; //Define node as pointer of data type struct LinkedList
SymbolNode createSymbolNode();
SymbolNode addSymbolNode(SymbolNode head, char *name, int value, enum SymbolType type, int isRelocatable);

#endif //MAMAN14_DATASTRUCTS_H
