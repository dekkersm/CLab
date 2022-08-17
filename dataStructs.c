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
        head = temp;
    }
    else
    {
        p  = head;
        while(p->next != NULL){
            if(!strcmp(p->name, name))
            {
                /*// A similar symbol already exists*/
                return NULL;
            }
            p = p->next;
        }
        p->next = temp;
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
    temp->next = NULL;
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
        head = temp;
    }
    else
    {
        p  = head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}
