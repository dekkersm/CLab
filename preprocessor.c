//
// Created by dekke on 09/07/2022.
//
#include "preprocessor.h"
#include "utils.h"

// Returns 1 if any macro statement is found, and changes the given macro flag
int CheckForMacroStatement(char line[], int *isMacroDefinition)
{
    char firstWord[MAX_CHARS_IN_LINE];
    memset(firstWord , '\0' , MAX_CHARS_IN_LINE);

    ReadFirstWordInLine(line, firstWord);

    if (!strcmp(firstWord, "macro")) {
        *isMacroDefinition = 1;
        return 1;
    }
    if (!strcmp(firstWord, "endmacro")) {
        *isMacroDefinition = 0;
        return 1;
    }
    return 0;
}

int ParseMacros(char *sourceFileName)
{
    char line [MAX_CHARS_IN_LINE];
    memset(line , '\0' , MAX_CHARS_IN_LINE);

    // The head of the macro list
    struct MacroNode* head = NULL;
    head = (struct MacroNode*)malloc(sizeof(struct MacroNode));
    memset(head->name, '\0', MAX_CHARS_IN_LINE);
    memset(head->content, '\0', MAX_CHARS_IN_LINE);
    head->next = NULL;

    struct MacroNode* realHead = head;

    // Opening the original source file
    FILE *sourceFile;
    sourceFile = fopen(sourceFileName,"r");
    if(sourceFile == NULL)
    {
        printf("ERROR: can't open the file: %s \n \n" , sourceFileName);
        return 1;
    }

    // Creating the new file
    FILE *amSourceFile;
    char amFileName[MAX_CHARS_IN_FILE_NAME];
    strcpy(amFileName, sourceFileName);
    strncat(amFileName, ".am", 4);
    amSourceFile = fopen(amFileName,"w");

    int isMacroDefinition = 0;

    // Temp var to hold the current macro being saved to table
    struct MacroNode* currMacro = NULL;
    currMacro = (struct MacroNode*)malloc(sizeof(struct MacroNode));
    memset(currMacro->content, '\0', MAX_CHARS_IN_LINE);

    while(fgets(line, MAX_CHARS_IN_LINE, sourceFile))
    {
        if(CheckForMacroStatement(line, &isMacroDefinition))
        {
            // If a macro statement was found
            if(isMacroDefinition)
            {
                AddMacroNameToTable(line, currMacro);
            }
            else
            {
                // End of macro, Add to the macro list
                struct MacroNode* temp = NULL;
                temp = (struct MacroNode*)malloc(sizeof(struct MacroNode));
                strcpy(temp->name, currMacro->name);
                strcpy(temp->content, currMacro->content);
                head->next = temp;
                head = temp;

                // Resetting the curr macro
                memset(currMacro->name, '\0', MAX_CHARS_IN_LINE);
                memset(currMacro->content, '\0', MAX_CHARS_IN_LINE);
            }
        }
        else
        {
            // If the line is a regular non-macro statement
            if (isMacroDefinition) {
                AddLineToMacroContent(line, currMacro);
            }
            else {
                if (!checkIfLineIsADefinedMacro(line, realHead, amSourceFile))
                {
                    // Adding the line as is to the .am file if it is not a macro name
                    fprintf(amSourceFile, "%s", line);
                }
            }
        }
    }
    printf("name: %s", head->name);
    printf("content: \n%s", head->content);
    return 0;
}

void AddMacroNameToTable(char line[], struct MacroNode *currMacro)
{
    const char whitespace[3] = " \t\n";
    char *token;

    token = strtok(line, whitespace);
    while(token!=NULL && !strcmp(token, "macro"))
    {
        token = strtok(NULL, whitespace);
    }
    strcpy(currMacro->name, token);
}

void AddLineToMacroContent(char line[], struct MacroNode *currMacro)
{
    strncat(currMacro->content, line, MAX_CHARS_IN_LINE);
}

int checkIfLineIsADefinedMacro(char line[], struct MacroNode *macroListHead, FILE *amSourceFile)
{
    char firstWord[MAX_CHARS_IN_LINE];
    memset(firstWord , '\0' , MAX_CHARS_IN_LINE);

    struct MacroNode* temp = macroListHead;

    ReadFirstWordInLine(line, firstWord);

    while(temp!=NULL)
    {
        if(!strcmp(temp->name, firstWord))
        {
            fprintf(amSourceFile, "%s", temp->content);
            return 1;
        }
        temp = temp->next;
    }

    return 0;
}