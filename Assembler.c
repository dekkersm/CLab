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
    if(fileName == NULL)
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
    // TODO: don't create if no extern
    FILE *extFile;
    extFile = fopen(fileName,"w");
    if(fileName == NULL)
    {
        printf("ERROR: can't open the file: \n \n");
    }

    char line[MAX_CHARS_IN_LINE];
    char value[MAX_CHARS_IN_LINE];

    ExternNode p;
    p = externList;
    while(p != NULL){
        memset(line, '\0', MAX_CHARS_IN_LINE);
        memset(value, '\0', MAX_CHARS_IN_LINE);

        sprintf(line, "%s %s", p->name, decTo32(value, p->value));
        fprintf(extFile, "%s\n", line);
        p = p->next;
    }

    fclose(extFile);
}

void writeToEntFile(SymbolNode symbolTable, char *fileName)
{
    // TODO: don't create if no entry
    FILE *entFile;
    entFile = fopen(fileName,"w");
    if(fileName == NULL)
    {
        printf("ERROR: can't open the file: \n \n");
    }

    char line[MAX_CHARS_IN_LINE];
    char value[MAX_CHARS_IN_LINE];

    SymbolNode p;
    p = symbolTable;
    while(p != NULL){
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

void compileFile(char *arg)
{

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
    int i = ParseMacros(argv[1]);
    FILE *sourceFile;
    sourceFile = fopen("C:\\May\\OpenU\\CLab\\maman14\\Docs\\source.txt.am","r");
    if(sourceFile == NULL)
    {
        printf("ERROR: can't open the file: \n \n");
        return 1;
    }

    // Initializing the symbol table to null
    SymbolNode table = NULL;
    table = addSymbolNode(table, "!", 0, external, 0);

    short instructionsArray[MEMORY_ARRAY_WORD_SIZE];
    short dataArray[MEMORY_ARRAY_WORD_SIZE];
    memset(instructionsArray, 0, sizeof(instructionsArray));
    memset(dataArray, 0, sizeof(dataArray));

    int DC = firstRunOnAssemblyFile(sourceFile, table, instructionsArray, dataArray);

    int j;
    for (j = 0; j < MEMORY_ARRAY_WORD_SIZE; ++j) {
        printf("array %d: %d\n", j, instructionsArray[j]);
    }

    // rewind the source file
    rewind(sourceFile);

    ExternNode externList = NULL;
    externList = addExternNode(externList, "k", 0);

    int IC = secondRunOnAssemblyFile(sourceFile, table, instructionsArray, externList);


    ExternNode p;
    p = externList;
    while(p != NULL){
        printf("name:%s, val:%d \n", p->name, p->value);
        p = p->next;
    }

    for (j = 0; j < MEMORY_ARRAY_WORD_SIZE; ++j) {
        printf("array %d: %d\n", j+1, instructionsArray[j]);
    }

    char res[100];
    printf("%s\n", decTo32(res, 123));

    char *objFileName = "C:\\May\\OpenU\\CLab\\maman14\\Docs\\source.txt.obj";
    char *extFileName = "C:\\May\\OpenU\\CLab\\maman14\\Docs\\source.txt.ext";
    char *entFileName = "C:\\May\\OpenU\\CLab\\maman14\\Docs\\source.txt.ent";
    writeToObjFile(IC, DC, instructionsArray, objFileName);
    writeToExtFile(externList, extFileName);
    writeToEntFile(table, entFileName);

    fclose(sourceFile);

//    assembler(argc, argv);
}