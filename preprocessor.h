#ifndef MAMAN14_PREPROCESSOR_H
#define MAMAN14_PREPROCESSOR_H

#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "utils.h"

struct MacroList {
    char name[MAX_CHARS_IN_LINE];
    char content[MAX_CHARS_IN_LINE * MAX_LINES_IN_MACRO];
    struct MacroList* next;
};

typedef struct MacroList *MacroNode;
MacroNode createMacroNode();
MacroNode addMacroNode(MacroNode head, char *name, char *content);
MacroNode getMacroByName(MacroNode head, char *name);

int CheckForMacroStatement(char line[], int *isMacroDefinition);
void ParseMacros(FILE *asFile, FILE *amFile);
void AddMacroNameToTable(char line[], char *currMacroName);
void AddLineToMacroContent(char line[], char *currMacroContent);
int checkIfLineIsADefinedMacro(char line[], MacroNode macroList, FILE *amSourceFile);

#endif
