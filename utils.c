//
// Created by dekke on 07/08/2022.
//
#include "utils.h"

void ReadFirstWordInLine(char line[], char *word)
{
    int lineIndex = 0;
    int wordIndex = 0;

    while(isspace(line[lineIndex]))
    {
        lineIndex++;
    }

    while(!isspace(line[lineIndex]))
    {
        word[wordIndex] = line[lineIndex];
        lineIndex++;
        wordIndex++;
    }
}