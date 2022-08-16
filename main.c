#include <stdio.h>
#include "preprocessor.h"
#include "firstRun.h"
#include "secondRun.h"

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

    firstRunOnAssemblyFile(sourceFile, table, instructionsArray, dataArray);

    int j;
    for (j = 0; j < MEMORY_ARRAY_WORD_SIZE; ++j) {
        printf("array %d: %d\n", j, instructionsArray[j]);
    }

    fclose(sourceFile);

    // open again
    sourceFile = fopen("C:\\May\\OpenU\\CLab\\maman14\\Docs\\source.txt.am","r");
    if(sourceFile == NULL)
    {
        printf("ERROR: can't open the file: \n \n");
        return 1;
    }

    ExternNode externList = NULL;
    externList = addExternNode(externList, "k", 0);

    secondRunOnAssemblyFile(sourceFile, table, instructionsArray, externList);


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

    fclose(sourceFile);
}
