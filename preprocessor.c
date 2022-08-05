//
// Created by dekke on 09/07/2022.
//
#include "preprocessor.h"

void ReadFirstWord(char line[], char word[])
{
    int lineIndex = 0;
    int wordIndex = 0;

    while(isspace(line[lineIndex]))
    {
        lineIndex ++;
    }

    while(!isspace(line[lineIndex]))
    {
        word[wordIndex] = line[lineIndex];
        lineIndex++;
        wordIndex++;
    }
}

void CheckForMacro(char line[], int *isMacro)
{
    char firstWord[MAX_CHARS_IN_LINE];
    memset(firstWord , '\0' , MAX_CHARS_IN_LINE);

    ReadFirstWord(line, firstWord);

    if (!strcmp(firstWord, "macro"))
        *isMacro = 1;
    if (!strcmp(firstWord, "endmacro"))
        *isMacro = 0;
}

int ParseMacros(int i, char *argv[])//, struct MacroNode *head)
{
    char line [MAX_CHARS_IN_LINE];
    memset(line , '\0' , MAX_CHARS_IN_LINE);

    FILE *sourceFile;
    sourceFile = fopen(argv[i],"r");

    int isMacro = 0;

    if(sourceFile == NULL)
    {
        printf("ERROR: can't open the file: %s \n \n" , argv[i]);
        return 1;
    }
    while(fgets(line, MAX_CHARS_IN_LINE, sourceFile))
    {
        // Parse line
        struct  MacroNode* temp = NULL;
        temp = (struct MacroNode*)malloc(sizeof(struct MacroNode));

        CheckForMacro(line, &isMacro);
        if(isMacro)
        {
            printf("macro!\n");
        }
        else
            printf("no macro\n");
    }
    return 0;
}