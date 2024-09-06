#include "global.h"

status Dpll(HeadNode *head, Literal *ltr, int valnum)
{
    HeadNode *trav = head;

    /*判断是否存在空子句*/
    while (trav)
    {
        if (trav->num == 0)
        {
            Destroy(head,ltr,valnum);
            return NORESULT;
        }
            
        trav = trav->next;
    }
    /*处理单子句*/
    while (CheckSolo(head) == TRUE)
    {
        SoloClause(head, ltr, valnum);
    }

    /*分裂法递归求解*/
    int i = 0;
    while (i < 2 * valnum)
    {
        if (ltr[i].next != nullptr)
            break;
    }
    if (i == 2 * valnum)
        return TRUE;
    if (head == nullptr)
        return TRUE;

    /* 向 原 CNF 结 构 体 尾 部 添 加 所 选 元 素 假 值 */
    for (trav = head; trav->next; trav = trav->next) ;

    trav->next = new HeadNode;
    trav->next->child = new Node;
    trav->next->next = nullptr;
    trav->next->num = 1;

    int data;
    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->next->child->data = data;//  <0 假值
    trav->next->child->next = nullptr;

    Literal *p = new Literal;
    p->pos = trav->next->child;
    p->pre = nullptr;
    p->parent = trav->next;
    p->uncle = trav;
    p->next = ltr[i].next;
    ltr[i].next = p->next;
    if (Dpll(head, ltr, valnum) == TRUE)
        return TRUE;

    /* 向 原 CNF 结 构 体 尾 部 添 加 所 选 元 素 真 值 */
    HeadNode *coh = Copy(head);
    Literal *col = Copy(ltr,valnum);

    for (trav = coh; trav->next; trav = trav->next) ;

    trav->next = new HeadNode;
    trav->next->child = new Node;
    trav->next->next = nullptr;
    trav->next->num = 1;

    int data;
    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->next->child->data = -data; //  >0 真值
    trav->next->child->next = nullptr;

    Literal *p = new Literal;
    p->pos = trav->next->child;
    p->pre = nullptr;
    p->parent = trav->next;
    p->uncle = trav;
    p->next = col[i].next;
    col[i].next = p->next;
    return Dpll(coh,col,valnum);
}