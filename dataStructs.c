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

SymbolNode addSymbolNode(SymbolNode head, char *name, int value, char *type, int isRelocatable)
{
    SymbolNode temp,p;// declare two nodes temp and p
    temp = createSymbolNode();//createNode will return a new node with data = value and next pointing to NULL.
// TODO: add error assert when symbol already exists
    temp->value = value;
    temp->isRelocatable = isRelocatable;
    strcpy(temp->type, type);
    strcpy(temp->name, name);

    if(head == NULL)
    {
        head = temp;     //when linked list is empty
    }
    else
    {
        p  = head;//assign head to p
        while(p->next != NULL){
            p = p->next;//traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp;//Point the previous last node to the new node created.
    }
    return head;
}
