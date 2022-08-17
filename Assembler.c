//
// Created by dekke on 16/08/2022.
//

#include "preprocessor.h"
#include "firstRun.h"
#include "secondRun.h"


void writeToObjFile(int IC, int DC, short memoryArray[], char *fileName)
{
    FILE *objFile;
    objFile = fopen(fileName,"w");
    if(objFile == NULL)
    {
        printf("ERROR: can't open the file: \n \n");
    }

    char line[MAX_CHARS_IN_LINE];
    sprintf(line, "%d %d", IC-1, DC);
    fprintf(objFile, "%s\n", line);

    char address[MAX_CHARS_IN_LINE];
    char value[MAX_CHARS_IN_LINE];

    int j = 0;
    for (j = 0; j < IC+DC; ++j) {
        memset(address, '\0', MAX_CHARS_IN_LINE);
        memset(value, '\0', MAX_CHARS_IN_LINE);
        memset(line, '\0', MAX_CHARS_IN_LINE);

        sprintf(line, "%s %s", decTo32(address,j+FIRST_ADDRESS_IN_OBJ_FILE), decTo32(value, memoryArray[j]));
        fprintf(objFile, "%s\n", line);
    }

    fclose(objFile);
}

void writeToExtFile(ExternNode externList, char *fileName)
{
    FILE *extFile = NULL;

    char line[MAX_CHARS_IN_LINE];
    char value[MAX_CHARS_IN_LINE];

    ExternNode p;
    p = externList->next;
    while(p != NULL){
        if(extFile == NULL)
        {
            // Create the file if not created yet
            extFile = fopen(fileName,"w");
            if(extFile == NULL)
            {
                printf("ERROR: can't open the file: \n \n");
            }
        }
        memset(line, '\0', MAX_CHARS_IN_LINE);
        memset(value, '\0', MAX_CHARS_IN_LINE);

        sprintf(line, "%s %s", p->name, decTo32(value, p->value));
        fprintf(extFile, "%s\n", line);
        printf("ext: %s\n", line);
        p = p->next;
    }

    fclose(extFile);
}

void writeToEntFile(SymbolNode symbolTable, char *fileName)
{
    FILE *entFile;

    char line[MAX_CHARS_IN_LINE];
    char value[MAX_CHARS_IN_LINE];

    SymbolNode p;
    p = symbolTable;
    while(p != NULL){
        if(entFile == NULL) {
            entFile = fopen(fileName, "w");
            if (entFile == NULL) {
                printf("ERROR: can't open the file: \n \n");
            }
        }
        memset(line, '\0', MAX_CHARS_IN_LINE);
        memset(value, '\0', MAX_CHARS_IN_LINE);

        if(p->type == entry) {
            sprintf(line, "%s %s", p->name, decTo32(value, p->value));
            fprintf(entFile, "%s\n", line);
        }
        p = p->next;
    }

    fclose(entFile);
}

void compileFile(char *fileName)
{
    FILE *asFile;
    char asFileName[MAX_CHARS_IN_FILE_NAME];
    sprintf(asFileName,"%s.as", fileName);

    asFile = fopen(asFileName,"r");
    if(asFile == NULL)
    {
        printf("ERROR: can't open the source file %s\n", asFileName);
    }
    else {
        FILE *amFile;
        char amFileName[MAX_CHARS_IN_FILE_NAME];
        sprintf(amFileName,"%s.am", fileName);
        amFile = fopen(amFileName,"w+");
        if(amFile == NULL)
        {
            printf("ERROR: can't open the source file %s\n", asFileName);
        }
        else {
            // Pre-processor
            ParseMacros(asFile, amFile);
            fclose(asFile);
            rewind(amFile);

            // Initializing the symbol table to null and memory array
            SymbolNode symbolTable = createSymbolNode();
            short memoryArray[MEMORY_ARRAY_WORD_SIZE];
            memset(memoryArray, 0, sizeof(memoryArray));

            int DC = firstRunOnAssemblyFile(amFile, symbolTable, memoryArray);
            if(DC >= 0) {
                // rewind the source file
                rewind(amFile);
                ExternNode externList = createExternNode();

                int IC = secondRunOnAssemblyFile(amFile, symbolTable, memoryArray, externList);
                if(IC >= 0){
                    char objFileName[MAX_CHARS_IN_FILE_NAME];
                    char extFileName[MAX_CHARS_IN_FILE_NAME];
                    char entFileName[MAX_CHARS_IN_FILE_NAME];
                    sprintf(objFileName,"%s.ob", fileName);
                    sprintf(extFileName,"%s.ext", fileName);
                    sprintf(entFileName,"%s.ent", fileName);

                    writeToObjFile(IC, DC, memoryArray, objFileName);
                    writeToExtFile(externList, extFileName);
                    writeToEntFile(symbolTable, entFileName);

                    fclose(amFile);
                }
            }
        }
    }
}

void assembler(int argc, char *argv[])
{
    int filesCount = argc - 1;
    int i = 1;
    if (filesCount < 1)
    {
        fprintf(stderr, "\n\nYou did not passed any source files to the assembler!\n\n");
        exit(1);
    }
    while (--argc)
    {
        compileFile(argv[i]);
        i++;
    }
}

int main(int argc, char *argv[]) {
    assembler(argc, argv);
}