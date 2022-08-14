//
// Created by dekke on 08/08/2022.
//
#include "string.h"
#include "firstRun.h"

int IC = 0;
int DC = 0;
int lineCounter = 0;
short instructionsArray[MEMORY_ARRAY_WORD_SIZE];
short dataArray[MEMORY_ARRAY_WORD_SIZE];

int parseCommand(char *cmd, Command *currCmd)
{
    int success = 1;
    if(!strcmp(cmd, "mov")){
        currCmd->opcode = 0;
        currCmd->operandCount = 2;

    }
    else if(!strcmp(cmd, "cmp")) {
        currCmd->opcode = 1;
        currCmd->operandCount = 2;
    }
    else if(!strcmp(cmd, "add")){
        currCmd->opcode = 2;
        currCmd->operandCount = 2;
    }
    else if(!strcmp(cmd, "sub")){
        currCmd->opcode = 3;
        currCmd->operandCount = 2;
    }
    else if(!strcmp(cmd, "not")){
        currCmd->opcode = 4;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "clr")){
        currCmd->opcode = 5;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "lea")){
        currCmd->opcode = 6;
        currCmd->operandCount = 2;
    }
    else if(!strcmp(cmd, "inc")){
        currCmd->opcode = 7;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "dec")){
        currCmd->opcode = 8;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "jmp")){
        currCmd->opcode = 9;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "bne")){
        currCmd->opcode = 10;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "get")){
        currCmd->opcode = 11;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "prn")){
        currCmd->opcode = 12;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "jsr")){
        currCmd->opcode = 13;
        currCmd->operandCount = 1;
    }
    else if(!strcmp(cmd, "rts")){
        currCmd->opcode = 14;
        currCmd->operandCount = 0;
    }
    else if(!strcmp(cmd, "hlt")){
        currCmd->opcode = 15;
        currCmd->operandCount = 0;
    }
    else
    {
        success = 0;
    }
    return success;
}

void parseAssemblyLines(FILE *amFile, SymbolNode symbolTable)
{
    int isCodeValid = 1; // Where any errors found

    // Initializing the memory arrays and counters
    IC = 0;
    DC = 0;
    lineCounter = 0;
    memset(instructionsArray, 0, MEMORY_ARRAY_WORD_SIZE);
    memset(dataArray, 0, MEMORY_ARRAY_WORD_SIZE);

    char currLine[MAX_CHARS_IN_LINE];
    memset(currLine , '\0' , MAX_CHARS_IN_LINE);

    while(fgets(currLine, MAX_CHARS_IN_LINE, amFile))
    {
        lineCounter++;
        printf("line :%d, ", lineCounter);
        parseLine(currLine, symbolTable);
    }

    if(isCodeValid)
    {
      // update the data in symbol table
      // and start run 2
    }

    // errors where found - halt operation
}

int isEmptyOrCommentLine(char *currLine)
{
    int lineIndex = 0;
    if(currLine[lineIndex] == ';')
    {
        return 1;
    }

    while(isspace(currLine[lineIndex]))
    {
        if(currLine[lineIndex] == '\0')
            return 1;
        lineIndex++;
    }
    return 0;
}

int isSymbolDeclaration(char *firstWord)
{
    int wordIndex = 0;
    if(isalpha(firstWord[wordIndex]))
    {
        wordIndex++;
        while(isalnum(firstWord[wordIndex]))
        {
            wordIndex++;
        }
        if(firstWord[wordIndex] == ':')
        {
            return 1;
        }
    }

    return 0;
}

int isGuidingLine(char const *currLine)
{
    int lineIndex = 0;

    while(isspace(currLine[lineIndex]))
    {
        lineIndex++;
    }

    return currLine[lineIndex] == '.';
}

int classifyGuidingType(char *currLine, enum GuidingType *guidingType)
{
    char dataTypeName[SYMBOL_MAX_CHAR_LENGTH];
    memset(dataTypeName, '\0', SYMBOL_MAX_CHAR_LENGTH);
    readFirstWordInLine(currLine, dataTypeName);

    if(!strcmp(dataTypeName, ".data"))
    {
        *guidingType = dataType;
    }
    else if(!strcmp(dataTypeName, ".string"))
    {
        *guidingType = stringType;
    }
    else if(!strcmp(dataTypeName, ".struct"))
    {
        *guidingType = structType;
    }
    else if(!strcmp(dataTypeName, ".extern"))
    {
        *guidingType = externalType;
    }
    else if(!strcmp(dataTypeName, ".entry"))
    {
        *guidingType = entryType;
    }
    else // if not any of the data types return false
    {
        return 0;
    }

    return 1;
}

char* getExternOperand(char *externLine)
{
    char *externOperand;
    externOperand = strtok(externLine, " \t\n"); // Dummy Read for the first word in line
    externOperand = strtok(NULL, " \t\n");

    return externOperand;
}

void parseLine(char *currLine, SymbolNode symbolTable)
{
    if(!isEmptyOrCommentLine(currLine))
    {
        char *currSymbol;
        int isSymbolDeclared = 0;
        char firstWordInCurrLine[SYMBOL_MAX_CHAR_LENGTH];
        memset(firstWordInCurrLine, '\0', SYMBOL_MAX_CHAR_LENGTH);
        readFirstWordInLine(currLine, firstWordInCurrLine);

        if (isSymbolDeclaration(firstWordInCurrLine)) {
            // Slice the symbol declaration from the line for further parsing as normal line
            currSymbol = strtok(firstWordInCurrLine, ":");
            currLine = strchr(currLine, ':')+1;
            isSymbolDeclared = 1;
            printf("line symbol:%s, ", currSymbol);
        }

        // Checking if the line is a guiding line or instruction
        if(isGuidingLine(currLine))
        {
            // Classify the type of guiding line, if not found raise error
            enum GuidingType guidingType;
            if (classifyGuidingType(currLine, &guidingType))
            {
                if (guidingType == entryType || guidingType == externalType)
                {
                    if(guidingType==externalType)
                    {
                        char *externOperand = getExternOperand(currLine);
                        if(externOperand)
                        {
                            printf("extern param: %s, ", externOperand);
                            addSymbolNode(symbolTable, externOperand, 0, external, 0);
                        }
                        // TODO: error raise on extern dont have operand
                    }
                }
                // Data types::::
                else
                {
                    if (isSymbolDeclared) {
                        addSymbolNode(symbolTable, currSymbol, DC, data, 1);
                        // TODO: if already in table - error assert
                    }

                    // TODO: insert data to dataArray and increase DC
                }
                printf("guiding data type : %d\n", guidingType);
            }
            else
            {
                // TODO: invalid guiding line
            }
        }
        else
        {
            if (isSymbolDeclared) {
                // insert symbol to table with IC
                // if already in table - error assert
            }

            char cmdName[SYMBOL_MAX_CHAR_LENGTH];
            memset(cmdName, '\0', SYMBOL_MAX_CHAR_LENGTH);
            readFirstWordInLine(currLine, cmdName);

            Command *currCmd = (Command*)malloc(sizeof(Command));

            if (parseCommand(cmdName, currCmd))
            {
                printf("instruction\n");
                int L = 0;
                // parse the instruction and compute L
                // build the first binary word of the instruction

//                IC = IC + L;
            }
        }
    }
    else
    {

        printf("empty line\n");
    }

}
