//
// Created by dekke on 16/08/2022.
//

#include "secondRun.h"

short secondRunIC = 0;
int secondRunLC = 0;
int externCount = 0;
const isSecondRun = 1;

int secondRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable, short instructionsArray[], ExternNode externList)
{
    int isCodeValid = 1; // Where any errors found

    // Initializing the counters
    secondRunIC = 0;
    secondRunLC = 0;
    externCount = 0;

    char currLine[MAX_CHARS_IN_LINE];
    memset(currLine , '\0' , MAX_CHARS_IN_LINE);

    while(fgets(currLine, MAX_CHARS_IN_LINE, amFile))
    {
        secondRunLC++;
        printf("\nline :%d, ", secondRunLC);
        parseLineSecondRun(currLine, symbolTable, instructionsArray, externList);
    }

    if(isCodeValid)
    {
        return secondRunIC;
    }

    // errors where found - halt operation
    return -1;
}

void parseLineSecondRun(char *currLine, SymbolNode symbolTable, short memoryArray[], ExternNode externList)
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
                short L = parseInstructionLineOperands(currLine, currCmd, symbolTable, memoryArray, externList);
                secondRunIC = secondRunIC + L;
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

int AddOperandWordToMemory(Operand *operand, short *currWord, int *L, int wasRegister, int isDestOperand, short memoryArray[], ExternNode externList)
{
    switch (operand->type) {
        case num:
        case symbol: {
            *currWord = BIT_MASK_FOR_WORD_SIZE & (operand->value << EXTRA_MEMORY_WORD_OFFSET | operand->are << ARE_WORD_OFFSET);
            printf("op:%d, ", operand->value);
            memoryArray[secondRunIC + *L] = *currWord;
            // Adding to extern table if needed
            if(operand->are == ext)
            {
                addExternNode(externList, operand->symbolName, secondRunIC + *L + FIRST_ADDRESS_IN_OBJ_FILE);
            }
            *L += 1;
            break;
        }
        case struc: {
            *currWord = BIT_MASK_FOR_WORD_SIZE & (operand->value << EXTRA_MEMORY_WORD_OFFSET | operand->are << ARE_WORD_OFFSET);
            short fieldWord = operand->fieldValue << EXTRA_MEMORY_WORD_OFFSET;
            memoryArray[secondRunIC + *L] = *currWord;
            memoryArray[secondRunIC + *L +1] = fieldWord;
            // Adding to extern table if needed
            if(operand->are == ext)
            {
                addExternNode(externList, operand->symbolName, secondRunIC + *L + FIRST_ADDRESS_IN_OBJ_FILE);
            }
            *L += 2;
            break;
        }
        case reg: {
            if(!wasRegister) *currWord=0;
            if(isDestOperand) {
                *currWord |= BIT_MASK_FOR_WORD_SIZE & (operand->value << DEST_REGISTER_WORD_OFFSET | operand->are << ARE_WORD_OFFSET);
                if(wasRegister){
                    memoryArray[secondRunIC + *L -1] = *currWord;
                }
                else
                {
                    memoryArray[secondRunIC + *L] = *currWord;
                }
            }
            else
            {
                *currWord |= BIT_MASK_FOR_WORD_SIZE & (operand->value << SRC_REGISTER_WORD_OFFSET | operand->are << ARE_WORD_OFFSET);
                memoryArray[secondRunIC + *L] = *currWord;
                *L += 1;
                wasRegister = 1;
            }
            if(!wasRegister) *L += 1;
            break;
        }
        default: break;
    }
    printf("curr word is:%d, L is:%d, IC is:%d, ", *currWord, *L, secondRunIC);
    return wasRegister;
}

short parseInstructionLineOperands(char *currLine, Command *currCmd, SymbolNode symbolTable, short memoryArray[], ExternNode externList)
{
    int L = 1;

    char *firstOperandString = strtok(currLine, ", \t\n"); // Dummy read of the instruction name
    firstOperandString = strtok(NULL, ", \t\n");

    int wasRegister = 0;
    short currWord = 0;

    if(firstOperandString != NULL) {
        int isDestOperand = currCmd->operandCount == 1;
        Operand *firstOperand = parseOperand(firstOperandString, symbolTable, 0);
        if(checkAddressingMethodValidity(firstOperand->addressingMethod,
                                         ( isDestOperand ?
                                           currCmd->destOpLegalAddressMethods :
                                           currCmd->sourceOpLegalAddressMethods)))
        {
            wasRegister = AddOperandWordToMemory(firstOperand, &currWord, &L, wasRegister, isDestOperand, memoryArray, externList);
        }
        else
        {
            // TODO: illegal operand addressing method
        }
    }

    // Parse second operand if needed
    if (currCmd->operandCount == 2) {
        char *destOperandString = strtok(NULL, ", \t\n");
        if(destOperandString != NULL) {
            Operand *destOperand = parseOperand(destOperandString, symbolTable, 0);
            if(checkAddressingMethodValidity(destOperand->addressingMethod, currCmd->destOpLegalAddressMethods))
            {
                AddOperandWordToMemory(destOperand, &currWord, &L, wasRegister, 1, memoryArray, externList);
            }
            else
            {
                // TODO: illegal operand addressing method
            }
        }
    }

    return L;
}