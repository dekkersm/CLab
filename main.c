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

    firstRunOnAssemblyFile(sourceFile, table);

    fclose(sourceFile);

    // open again
    sourceFile = fopen("C:\\May\\OpenU\\CLab\\maman14\\Docs\\source.txt.am","r");
    if(sourceFile == NULL)
    {
        printf("ERROR: can't open the file: \n \n");
        return 1;
    }

    secondRunOnAssemblyFile(sourceFile, table, instructionsArray, dataArray);

    fclose(sourceFile);
}
