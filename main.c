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
    table = addSymbolNode(table, "prn", 300, external, 1);

    parseAssemblyLines(sourceFile, table);

    SymbolNode p;
    p = table;
    while(p != NULL){
        printf("name: %s, value: %d, type: %d, is: %d\n", p->name, p->value, p->type, p->isRelocatable);
        p = p->next;
    }

    printf("is: %d", stringToInt("14"));
    fclose(sourceFile);
}
