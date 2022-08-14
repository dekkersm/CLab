//
// Created by dekke on 14/08/2022.
//

#include "dataStructs.h"

SymbolNode createSymbolNode(){
    SymbolNode temp; // declare a node
    temp = (SymbolNode)malloc(sizeof(struct SymbolTable)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

SymbolNode addSymbolNode(SymbolNode head, char *name, int value, enum SymbolType type, int isRelocatable)
{
    SymbolNode temp,p;
    temp = createSymbolNode();
    temp->value = value;
    temp->isRelocatable = isRelocatable;
    temp->type = type;
    strcpy(temp->name, name);

    if(head == NULL)
    {
        head = temp;     //when linked list is empty
    }
    else
    {
        p  = head;
        while(p->next != NULL){
            // TODO: Check if name exists already
            p = p->next;//traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp;//Point the previous last node to the new node created.
    }
    return head;
}
