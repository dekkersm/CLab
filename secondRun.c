#include "secondRun.h"

short secondRunIC = 0;
int secondRunLC = 0;
int isCodeValid = 1;
const isSecondRun = 1;

int secondRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable, short instructionsArray[], ExternNode externList)
{
    /*// Initializing the counters*/
    secondRunIC = 0;
    secondRunLC = 0;

    char currLine[MAX_CHARS_IN_LINE];
    memset(currLine , '\0' , MAX_CHARS_IN_LINE);

    while(fgets(currLine, MAX_CHARS_IN_LINE, amFile))
    {
        secondRunLC++;
        parseLineSecondRun(currLine, symbolTable, instructionsArray, externList);
    }

    if(isCodeValid)
    {
        return secondRunIC;
    }

    /*// errors where found - halt operation*/
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
            /*// Slice the symbol declaration from the line for further parsing as normal line*/
            currLine = strchr(currLine, ':')+1;
        }

        /*// Checking if the line is a guiding line or instruction*/
        if(isGuidingLine(currLine))
        {
            /*// Classify the type of guiding line, if not found raise error*/
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
                printf("ERROR: guiding line not valid! line:%d", secondRunLC);
                isCodeValid = 0;
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
                /*// parse the instruction and compute L*/
                short L = parseInstructionLineOperands(currLine, currCmd, symbolTable, memoryArray, externList);
                secondRunIC = secondRunIC + L;
            }
            else
            {
                printf("ERROR: unrecognized command name! line:%d", secondRunLC);
                isCodeValid = 0;
            }
        }
    }
}

void parseEntryLine(char *currLine, SymbolNode symbolTable)
{
    char *entryOperand = getExternOrEntryOperand(currLine);
    SymbolNode currSymbol = getSymbolByName(symbolTable, entryOperand);

    if(currSymbol)
    {
        currSymbol->type = entry;
    }
    else {
        printf("ERROR: entry declaration of non-existing symbol! line:%d", secondRunLC);
        isCodeValid = 0;
    }
}

int AddOperandWordToMemory(Operand *operand, short *currWord, int *L, int wasRegister, int isDestOperand, short memoryArray[], ExternNode externList)
{
    switch (operand->type) {
        case num:
        case symbol: {
            *currWord = BIT_MASK_FOR_WORD_SIZE & (operand->value << EXTRA_MEMORY_WORD_OFFSET | operand->are << ARE_WORD_OFFSET);
            memoryArray[secondRunIC + *L] = *currWord;
            /*// Adding to extern table if needed*/
            if(operand->are == ext)
            {
                externList = addExternNode(externList, operand->symbolName, secondRunIC + *L + FIRST_ADDRESS_IN_OBJ_FILE);
            }
            *L += 1;
            break;
        }
        case struc: {
            *currWord = BIT_MASK_FOR_WORD_SIZE & (operand->value << EXTRA_MEMORY_WORD_OFFSET | operand->are << ARE_WORD_OFFSET);
            short fieldWord = operand->fieldValue << EXTRA_MEMORY_WORD_OFFSET;
            memoryArray[secondRunIC + *L] = *currWord;
            memoryArray[secondRunIC + *L +1] = fieldWord;
            /*// Adding to extern table if needed*/
            if(operand->are == ext)
            {
                externList = addExternNode(externList, operand->symbolName, secondRunIC + *L + FIRST_ADDRESS_IN_OBJ_FILE);
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
        Operand *firstOperand = parseOperand(firstOperandString, symbolTable, 0, secondRunLC);
        if(firstOperand) {
            if (checkAddressingMethodValidity(firstOperand->addressingMethod,
                                              (isDestOperand ?
                                               currCmd->destOpLegalAddressMethods :
                                               currCmd->sourceOpLegalAddressMethods))) {
                wasRegister = AddOperandWordToMemory(firstOperand, &currWord, &L, wasRegister, isDestOperand,
                                                     memoryArray, externList);
            } else {
                printf("ERROR: illegal operand addressing method! line:%d", secondRunLC);
                isCodeValid = 0;
            }
        }
        else
        {
            isCodeValid = 0;
        }
    }

    /*// Parse second operand if needed*/
    if (currCmd->operandCount == 2) {
        char *destOperandString = strtok(NULL, ", \t\n");
        if(destOperandString != NULL) {
            Operand *destOperand = parseOperand(destOperandString, symbolTable, 0, secondRunLC);
            if(destOperand) {
                if (checkAddressingMethodValidity(destOperand->addressingMethod, currCmd->destOpLegalAddressMethods)) {
                    AddOperandWordToMemory(destOperand, &currWord, &L, wasRegister, 1, memoryArray, externList);
                } else {
                    printf("ERROR: illegal operand addressing method! line:%d", secondRunLC);
                    isCodeValid = 0;
                }
            }
            else
            {
                isCodeValid = 0;
            }
        }
    }

    return L;
}