//
// Created by dekke on 07/08/2022.
//
#include "utils.h"

void readFirstWordInLine(char line[], char *word)
{
    int lineIndex = 0;
    int wordIndex = 0;

    while(isspace(line[lineIndex]) && line[lineIndex] != '\0')
    {
        lineIndex++;
    }

    while(!isspace(line[lineIndex]) && line[lineIndex] != '\0')
    {
        word[wordIndex] = line[lineIndex];
        lineIndex++;
        wordIndex++;
    }
}

int stringToInt(char *number)
{
    char p = *number;
    int intNumber = 0;
    int sign = 1;

    if ( '-' == *number || '+' == *number) {
        if ( '-' == *number) {
            sign = -1;
        }
        number++;
    }

    while (*number) {
        p = *number - '0';
        if ( 0 <= p && 9 >= p) {// digit 0 to 9
            intNumber = intNumber * 10 + p;
            number++;
        }
        else {
            break;//not a digit
            // TODO: error raise - not a digit where should be
        }
    }

    return intNumber * sign;
}