#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

void Destroy(HeadNode *head, Literal *ltr, int valnum)
{
    if(head == nullptr)
        return;
    HeadNode *trav = head;
    HeadNode *next = head->next;
    while (next)
    {
        DeleteNode(ltr, head, valnum);
        delete head;
        head = next;
        next = next->next;
    }
    DeleteNode(ltr, head, valnum);
    delete head;
    head = next;
}