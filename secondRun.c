//
// Created by dekke on 16/08/2022.
//

#include "secondRun.h"

int secondRunIC = 0;

void secondRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable, short instructionsArray[], short dataArray[])
{
    int isCodeValid = 1; // Where any errors found

    // Initializing the memory arrays and counters
    secondRunIC = 0;
    int lineCounter = 0;

    char currLine[MAX_CHARS_IN_LINE];
    memset(currLine , '\0' , MAX_CHARS_IN_LINE);

    while(fgets(currLine, MAX_CHARS_IN_LINE, amFile))
    {
        lineCounter++;
        printf("\nline :%d, ", lineCounter);
        parseLineSecondRun(currLine, symbolTable);
    }

    if(isCodeValid)
    {
    }

    // errors where found - halt operation
}

void parseLineSecondRun(char *currLine, SymbolNode symbolTable)
{
    if(!isEmptyOrCommentLine(currLine))
    {
        char *currSymbol;
        char firstWordInCurrLine[SYMBOL_MAX_CHAR_LENGTH];
        memset(firstWordInCurrLine, '\0', SYMBOL_MAX_CHAR_LENGTH);
        readFirstWordInLine(currLine, firstWordInCurrLine);

        if (isSymbolDeclaration(firstWordInCurrLine)) {
            // Slice the symbol declaration from the line for further parsing as normal line
            currLine = strchr(currLine, ':')+1;
        }

        // Checking if the line is a guiding line or instruction
        if(isGuidingLine(currLine))
        {
            // Classify the type of guiding line, if not found raise error
            enum GuidingType guidingType;
            if (classifyGuidingType(currLine, &guidingType))
            {
                if (guidingType == entryType)
                {
                    parseEntryLine(currLine, symbolTable);
                }
            }
            else
            {
                // TODO: ERROR invalid guiding line
            }
        }
        else
        {
            char cmdName[SYMBOL_MAX_CHAR_LENGTH];
            memset(cmdName, '\0', SYMBOL_MAX_CHAR_LENGTH);
            readFirstWordInLine(currLine, cmdName);

            Command *currCmd = (Command*)malloc(sizeof(Command));

            if (parseCommand(cmdName, currCmd))
            {
                printf("instruction, ");

                // parse the instruction and compute L
//                int L = parseInstructionLine(currLine, currCmd, symbolTable);
//                IC = IC + L;
            }
            else
            {
                // TODO: error in the cmd name
            }
        }
    }
    else
    {
        printf("empty line");
    }
}


void parseEntryLine(char *currLine, SymbolNode symbolTable)
{
    char *entryOperand = getExternOrEntryOperand(currLine);
    SymbolNode currSymbol = getSymbolByName(symbolTable, entryOperand);

    if(currSymbol)
    {
        printf("entry param: %s, ", entryOperand);
        currSymbol->type = entry;
    }
    else {
        // TODO: error raise entry of non existing symbol
    }
}