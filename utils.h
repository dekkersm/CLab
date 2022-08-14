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
int convertBase(int number,int base);
void toBase32(int number, char *numIn32);

#endif //MAMAN14_UTILS_H
