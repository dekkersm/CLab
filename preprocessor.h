//
// Created by dekke on 05/08/2022.
//

#ifndef MAMAN14_PREPROCESSOR_H
#define MAMAN14_PREPROCESSOR_H

#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "utils.h"

// TODO: make it good
struct MacroNode {
    char name[MAX_CHARS_IN_LINE];
    char content[MAX_CHARS_IN_LINE * MAX_LINES_IN_MACRO];
    struct MacroNode* next;
};

int CheckForMacroStatement(char line[], int *isMacroDefinition);
int ParseMacros(char *sourceFileName);
void AddMacroNameToTable(char line[], struct MacroNode *currMacro);
void AddLineToMacroContent(char line[], struct MacroNode *currMacro);
int checkIfLineIsADefinedMacro(char line[], struct MacroNode *macroListHead, FILE *amSourceFile);

#endif //MAMAN14_PREPROCESSOR_H
