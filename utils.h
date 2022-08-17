#ifndef MAMAN14_UTILS_H
#define MAMAN14_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.h"

void readFirstWordInLine(char line[], char *word);
int stringToInt(char *number, int lineCounter);
char* decTo32(char res[], short inputNum);

#endif
