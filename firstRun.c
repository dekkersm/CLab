//
// Created by dekke on 08/08/2022.
//
#include "string.h"
#include "firstRun.h"

short IC = 0;
short DC = 0;
int firstRunLC = 0;
const int isFirstRun = 1;

void appendToDataArray(short data, short dataArray[])
{
    dataArray[DC] = BIT_MASK_FOR_WORD_SIZE & data;
    DC++;
}

int firstRunOnAssemblyFile(FILE *amFile, SymbolNode symbolTable,short memoryArray[], short dataArray[])
{
    int isCodeValid = 1; // Where any errors found

    // Initializing the memory arrays and counters
    IC = 0;
    DC = 0;
    firstRunLC = 0;

    char currLine[MAX_CHARS_IN_LINE];
    memset(currLine , '\0' , MAX_CHARS_IN_LINE);

    while(fgets(currLine, MAX_CHARS_IN_LINE, amFile))
    {
        firstRunLC++;
        printf("\nline :%d, ", firstRunLC);
        parseLineFirstRun(currLine, symbolTable, memoryArray, dataArray);
    }

    if(isCodeValid)
    {
        // Going over the symbol table and adding the first address in memory to the counter, plus adding the IC to the data symbols
        SymbolNode p;
        p = symbolTable;
        while(p != NULL){
            if(p->type == data){
                p->value += IC + FIRST_ADDRESS_IN_OBJ_FILE;
            }
            else if(p->type == code) {
                p->value += FIRST_ADDRESS_IN_OBJ_FILE;
            }
            printf("\nname: %s, value: %d, type: %d, is: %d\n", p->name, p->value, p->type, p->isRelocatable);
            p = p->next;
        }

        // Going over the DC and adding all the data to the memory array
        int currDC = 0;
        while(currDC <= DC)
        {
            memoryArray[IC+currDC] = dataArray[currDC];
            currDC++;
        }
      // and start run 2
        return DC;
    }

    // errors where found - halt operation
    return -1;
}

void parseLineFirstRun(char *currLine, SymbolNode symbolTable, short memoryArray[], short dataArray[])
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
                        parseExternLine(currLine, symbolTable);
                    }
                }
                // Data types Handling
                else
                {
                    if (isSymbolDeclared) {
                        addSymbolNode(symbolTable, currSymbol, DC, data, 1);
                    }

                    switch (guidingType) {
                        case dataType: parseDataTypeLine(currLine, dataArray); break;
                        case stringType: parseStringTypeLine(currLine, dataArray); break;
                        case structType: parseStructTypeLine(currLine, dataArray); break;
                        default: break;
                    }
                }
            }
            else
            {
                // TODO: ERROR invalid guiding line
            }
        }
        else
        {
            if (isSymbolDeclared) {
                addSymbolNode(symbolTable, currSymbol, IC, code, 1);
            }

            char cmdName[SYMBOL_MAX_CHAR_LENGTH];
            memset(cmdName, '\0', SYMBOL_MAX_CHAR_LENGTH);
            readFirstWordInLine(currLine, cmdName);

            Command *currCmd = (Command*)malloc(sizeof(Command));

            if (parseCommand(cmdName, currCmd))
            {
                printf("instruction, ");

                // parse the instruction and compute L
                int L = parseInstructionLine(currLine, currCmd, symbolTable, memoryArray);
                IC = IC + L;
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

void parseDataTypeLine(char *currLine, short dataArray[])
{
    int lineIndex = 0;
    char currNum[MAX_CHARS_IN_DATA_NUM];
    memset(currNum, '\0', MAX_CHARS_IN_DATA_NUM);

    int wasComma = 0;
    int stopSign = 0;

    // Skip the .data declaration
    while(isspace(currLine[lineIndex]))
    {
        lineIndex++;
    }
    lineIndex += strlen(".data");

    while(!stopSign)
    {
        if(currLine[lineIndex] == ',' && !wasComma) {
            int operandNum = stringToInt(currNum);
            printf("num is: %d ", operandNum);
            appendToDataArray((short) operandNum, dataArray);
            wasComma = 1;
            memset(currNum, '\0', MAX_CHARS_IN_DATA_NUM);
        }
        else if(isdigit(currLine[lineIndex]) || currLine[lineIndex] == '-' || currLine[lineIndex] == '+')
        {
            wasComma = 0;
            strncat(currNum, &currLine[lineIndex], 1);
        }
        else if(currLine[lineIndex] == '\n' && !wasComma)
        {
            int operandNum = stringToInt(currNum);
            printf("num is: %d ", operandNum);
            appendToDataArray((short) operandNum, dataArray);
            stopSign = 1;
        }
        else if(!isspace(currLine[lineIndex]))
        {
            printf("ERROR in data ");
            // TODO: ERROR RAISE
        }
        lineIndex++;
    }
}

void parseStringTypeLine(char *currLine, short dataArray[])
{
    int lineIndex = 0;
    char string[MAX_CHARS_IN_LINE];
    memset(string, '\0', MAX_CHARS_IN_LINE);

    // Skip the .string declaration
    while(isspace(currLine[lineIndex]))
    {
        lineIndex++;
    }
    lineIndex += strlen(".string");

    int isInString = 0;

    while(currLine[lineIndex] != '\n')
    {
        if(!isInString && currLine[lineIndex] == '"')
        {
            isInString = ~isInString;
        }
        else if(isInString && currLine[lineIndex] == '"')
        {
            printf("/0 ");
            appendToDataArray('\0', dataArray);
            isInString = ~isInString;
        }
        else if(isInString)
        {
            printf("%c ", currLine[lineIndex]);
            appendToDataArray(currLine[lineIndex], dataArray);
        }
        else if(!isspace(currLine[lineIndex]))
        {
            printf("ERROR in string ");
            // TODO: error raise
        }
        lineIndex++;
    }
}

void parseStructTypeLine(char *currLine, short dataArray[])
{
    int lineIndex = 0;
    char currNum[MAX_CHARS_IN_DATA_NUM];
    memset(currNum, '\0', MAX_CHARS_IN_DATA_NUM);
    char string[MAX_CHARS_IN_LINE];
    memset(string, '\0', MAX_CHARS_IN_LINE);

    // Skip the .struct declaration
    while(isspace(currLine[lineIndex]))
    {
        lineIndex++;
    }
    lineIndex += strlen(".struct");

    while (currLine[lineIndex] != ',')
    {
        if(isdigit(currLine[lineIndex]) || currLine[lineIndex] == '-' || currLine[lineIndex] == '+')
        {
            strncat(currNum, &currLine[lineIndex], 1);
        }
        else if(!isspace(currLine[lineIndex]))
        {
            //error raise
        }
        lineIndex++;
    }

    // append the read num to the array
    int operandNum = stringToInt(currNum);
    printf("struct num is: %d ", operandNum);
    appendToDataArray((short) operandNum, dataArray);

    // Start the string handling
    lineIndex++; // For the un-read comma
    int isInString = 0;

    while(currLine[lineIndex] != '\n')
    {
        if(!isInString && currLine[lineIndex] == '"')
        {
            isInString = ~isInString;
        }
        else if(isInString && currLine[lineIndex] == '"')
        {
            printf("/0 ");
            appendToDataArray('\0', dataArray);
            isInString = ~isInString;
        }
        else if(isInString)
        {
            printf("char:%c, ", currLine[lineIndex]);
            appendToDataArray(currLine[lineIndex], dataArray);
        }
        else if(!isspace(currLine[lineIndex]))
        {
            printf("ERROR in struct string");
            // TODO: error raise
        }
        lineIndex++;
    }
}

void parseExternLine(char *currLine, SymbolNode symbolTable)
{
    char *externOperand = getExternOrEntryOperand(currLine);
    if(externOperand)
    {
        printf("extern param: %s, ", externOperand);
        addSymbolNode(symbolTable, externOperand, 0, external, 0);
    }
    // TODO: error raise on extern dont have operand
}

int parseInstructionLine(char *currLine, Command *currCmd, SymbolNode symbolTable, short memoryArray[])
{
    int L = 1;
    short currInstructionWord = 0;
    currInstructionWord |= absolute << ARE_WORD_OFFSET;
    currInstructionWord |= currCmd->opcode << OPCODE_WORD_OFFSET;

    char *firstOperandString = strtok(currLine, ", \t\n"); // Dummy read of the instruction name
    firstOperandString = strtok(NULL, ", \t\n");
    char *destOperandString = strtok(NULL, ", \t\n");

    // TODO: Error raise in case operand count is wrong
    switch (currCmd->operandCount) {
        case 0: if(firstOperandString || destOperandString) { printf("error");} break;
        case 1: if(!firstOperandString || destOperandString) { printf("error");} break;
        case 2: if(!firstOperandString || !destOperandString) { printf("error");} break;
        default: break;
    }

    Operand *firstOperand;
    Operand *destOperand;
    int wasRegister = 0;

    if(firstOperandString != NULL) {
        firstOperand = parseOperand(firstOperandString, symbolTable, isFirstRun);
        if(checkAddressingMethodValidity(firstOperand->addressingMethod,
                                         (currCmd->operandCount == 2 ?
                                         currCmd->sourceOpLegalAddressMethods :
                                         currCmd->destOpLegalAddressMethods)))
        {
            short addressingMethod;
            switch (firstOperand->type) {
                case num : L+=1; addressingMethod = 0; break;
                case symbol: L+=1; addressingMethod = 1; break;
                case struc: L+=2; addressingMethod = 2; break;
                case reg:
                {
                    L+=1;
                    addressingMethod = 3;
                    wasRegister = 1;
                    break;
                }
                default: break;
            }

            if(currCmd->operandCount == 2) {
                currInstructionWord |= addressingMethod << SRC_OPERAND_WORD_OFFSET;
            }
            else if(currCmd->operandCount == 1)
            {
                currInstructionWord |= addressingMethod << DEST_OPERAND_WORD_OFFSET;
            }
        }
        else
        {
            // TODO: illegal operand addressing method
        }
    }

    if(destOperandString != NULL) {
        destOperand = parseOperand(destOperandString, symbolTable, isFirstRun);
        if(checkAddressingMethodValidity(destOperand->addressingMethod, currCmd->destOpLegalAddressMethods))
        {
            short addressingMethod;
            switch (destOperand->type) {
                case num : L+=1; addressingMethod = 0; break;
                case symbol: L+=1; addressingMethod = 1; break;
                case struc: L+=2; addressingMethod = 2; break;
                case reg:
                {
                    addressingMethod = 3;
                    if(!wasRegister) L+=1;
                    break;
                }
                default: break;
            }
            currInstructionWord |= addressingMethod << DEST_OPERAND_WORD_OFFSET;
        }
        else
        {
            // TODO: illegal operand addressing method
        }
    }

    // Appending the first instruction word to the instruction array
    printf("word in memory %d", currInstructionWord);
    memoryArray[IC] = currInstructionWord;

    return L;
}
