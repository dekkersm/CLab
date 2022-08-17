#include "preprocessor.h"

MacroNode createMacroNode(){
    MacroNode temp;
    temp = (MacroNode)malloc(sizeof(struct MacroList));
    temp->next = NULL;
    return temp;
}

MacroNode addMacroNode(MacroNode head, char *name, char *content)
{
    MacroNode temp,p;
    temp = createMacroNode();
    strcpy(temp->name, name);
    strcpy(temp->content, content);

    if(head == NULL)
    {
        head = temp;
    }
    else
    {
        p  = head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

MacroNode getMacroByName(MacroNode head, char *name) {
    MacroNode p;
    p = head;
    while(p != NULL){
        if(!strcmp(p->name, name))
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

/*// Returns 1 if any macro statement is found, and changes the given macro flag*/
int CheckForMacroStatement(char line[], int *isMacroDefinition)
{
    char firstWord[MAX_CHARS_IN_LINE];
    memset(firstWord , '\0' , MAX_CHARS_IN_LINE);

    readFirstWordInLine(line, firstWord);

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

void ParseMacros(FILE *asFile, FILE *amFile)
{
    char line [MAX_CHARS_IN_LINE];
    memset(line , '\0' , MAX_CHARS_IN_LINE);

    /*// The head of the macro list*/
    MacroNode macroList = NULL;

    int isMacroDefinition = 0;

    char currMacroName[MAX_CHARS_IN_LINE];
    char currMacroContent[MAX_CHARS_IN_LINE];
    memset(currMacroName, '\0', MAX_CHARS_IN_LINE);
    memset(currMacroContent, '\0', MAX_CHARS_IN_LINE);

    while(fgets(line, MAX_CHARS_IN_LINE, asFile))
    {
        if(CheckForMacroStatement(line, &isMacroDefinition))
        {
            /*// If a macro statement was found*/
            if(isMacroDefinition)
            {
                AddMacroNameToTable(line, currMacroName);
            }
            else
            {
                /*// End of macro, Add to the macro list*/
                macroList = addMacroNode(macroList, currMacroName, currMacroContent);

                /*// Resetting the curr macro*/
                memset(currMacroName, '\0', MAX_CHARS_IN_LINE);
                memset(currMacroContent, '\0', MAX_CHARS_IN_LINE);
            }
        }
        else
        {
            /*// If the line is a regular non-macro statement*/
            if (isMacroDefinition) {
                AddLineToMacroContent(line, currMacroContent);
            }
            else {
                if (!checkIfLineIsADefinedMacro(line, macroList, amFile))
                {
                    /*// Adding the line as is to the .am file if it is not a macro name*/
                    fprintf(amFile, "%s", line);
                }
            }
        }
    }

    fprintf(amFile, "%c", '\n'); /*// new line in end of file*/
}

void AddMacroNameToTable(char line[], char *currMacroName)
{
    const char whitespace[3] = " \t\n";
    char *token;

    token = strtok(line, whitespace);
    while(token!=NULL && !strcmp(token, "macro"))
    {
        token = strtok(NULL, whitespace);
    }
    strcpy(currMacroName, token);
}

void AddLineToMacroContent(char line[], char *currMacroContent)
{
    strncat(currMacroContent, line, MAX_CHARS_IN_LINE);
}

int checkIfLineIsADefinedMacro(char line[], MacroNode macroList, FILE *amSourceFile)
{
    char firstWord[MAX_CHARS_IN_LINE];
    memset(firstWord , '\0' , MAX_CHARS_IN_LINE);
    readFirstWordInLine(line, firstWord);

    MacroNode temp = getMacroByName(macroList, firstWord);
    if(temp!=NULL)
    {
        fprintf(amSourceFile, "%s", temp->content);
        return 1;
    }

    return 0;
}