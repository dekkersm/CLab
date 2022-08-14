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