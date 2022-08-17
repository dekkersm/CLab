//
// Created by dekke on 14/08/2022.
//

#include "dataStructs.h"

SymbolNode createSymbolNode(){
    SymbolNode temp;
    temp = (SymbolNode)malloc(sizeof(struct SymbolTable));
    temp->next = NULL;
    return temp;
}

SymbolNode addSymbolNode(SymbolNode head, char *name, short value, SymbolType type, int isRelocatable)
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

SymbolNode getSymbolByName(SymbolNode head, char *name)
{
    SymbolNode p;
    p = head;
    while(p != NULL){
        if(!strcmp(p->name, name))
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

ExternNode createExternNode(){
    ExternNode temp;
    temp = (ExternNode)malloc(sizeof(struct ExternTable));
    temp->next = NULL;// make next point to NULL
    return temp;
}

ExternNode addExternNode(ExternNode head, char *name, short value)
{
    ExternNode temp,p;
    temp = createExternNode();
    temp->value = value;
    strcpy(temp->name, name);

    if(head == NULL)
    {
        head = temp;     //when linked list is empty
    }
    else
    {
        p  = head;
        while(p->next != NULL){
            p = p->next;//traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp;//Point the previous last node to the new node created.
    }
    return head;
}
