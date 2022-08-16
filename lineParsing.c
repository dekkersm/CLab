//
// Created by dekke on 16/08/2022.
//

#include "lineParsing.h"

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

char* getExternOrEntryOperand(char *currLine)
{
    char *operand;
    operand = strtok(currLine, " \t\n"); // Dummy Read for the first word in line
    operand = strtok(NULL, " \t\n");

    return operand;
}

int parseCommand(char *cmd, Command *currCmd)
{
    int success = 1;
    if(!strcmp(cmd, "mov")){
        currCmd->opcode = 0;
        currCmd->operandCount = 2;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){1,1,1,1};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "cmp")) {
        currCmd->opcode = 1;
        currCmd->operandCount = 2;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){1,1,1,1};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){1,1,1,1};
    }
    else if(!strcmp(cmd, "add")){
        currCmd->opcode = 2;
        currCmd->operandCount = 2;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){1,1,1,1};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "sub")){
        currCmd->opcode = 3;
        currCmd->operandCount = 2;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){1,1,1,1};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "not")){
        currCmd->opcode = 4;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "clr")){
        currCmd->opcode = 5;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "lea")){
        currCmd->opcode = 6;
        currCmd->operandCount = 2;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,1,1,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "inc")){
        currCmd->opcode = 7;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "dec")){
        currCmd->opcode = 8;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "jmp")){
        currCmd->opcode = 9;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "bne")){
        currCmd->opcode = 10;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "get")){
        currCmd->opcode = 11;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "prn")){
        currCmd->opcode = 12;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){1,1,1,1};
    }
    else if(!strcmp(cmd, "jsr")){
        currCmd->opcode = 13;
        currCmd->operandCount = 1;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,1,1,1};
    }
    else if(!strcmp(cmd, "rts")){
        currCmd->opcode = 14;
        currCmd->operandCount = 0;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
    }
    else if(!strcmp(cmd, "hlt")){
        currCmd->opcode = 15;
        currCmd->operandCount = 0;
        currCmd->sourceOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
        currCmd->destOpLegalAddressMethods = (AddressingMethod){0,0,0,0};
    }
    else
    {
        success = 0;
    }
    return success;
}