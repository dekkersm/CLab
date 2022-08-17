//
// Created by dekke on 07/08/2022.
//

#ifndef MAMAN14_UTILS_H
#define MAMAN14_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.h"

void readFirstWordInLine(char line[], char *word);
int stringToInt(char *number);
char* decTo32(char res[], short inputNum);

#endif //MAMAN14_UTILS_H
