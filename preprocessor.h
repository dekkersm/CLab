//
// Created by dekke on 05/08/2022.
//

#ifndef MAMAN14_PREPROCESSOR_H
#define MAMAN14_PREPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

struct MacroNode {
    char mname[MAX_CHARS_IN_LINE];
    char mcontent[MAX_CHARS_IN_LINE];
    struct MacroNode* next;
};

void ReadFirstWord(char line[], char word[]);
void CheckForMacro(char line[], int *isMacro);
int ParseMacros(int i, char *argv[]);//,struct MacroNode *head);

#endif //MAMAN14_PREPROCESSOR_H
