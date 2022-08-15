#include <stdio.h>
#include "dataStructs.h"
#include "preprocessor.h"
#include "firstRun.h"

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

    parseAssemblyLines(sourceFile, table);

    fclose(sourceFile);
}
