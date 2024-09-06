#include "global.h"

void Destroy(HeadNode *head, Literal *ltr, int valnum)
{
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