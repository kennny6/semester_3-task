#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

status Dpll(HeadNode *&head, Literal *ltr, int valnum, int *res, int &flag)
{
    HeadNode *trav = head;

    /* 处理单子句 */
    while (flag == TRUE)
    {
        int outcome = SoloClause(head, ltr, valnum, res, flag);
        if (outcome == FALSE)
            return FALSE;
    }

    /* 分裂法递归求解 */
    int n = 0,max = 0,i;
    while (n < 2 * valnum)
    {
        if(max < ltr[n].n)
        {
            max = ltr[n].n;
            i = n;
        }
        n++;
    }
    if (max == 0)
        return TRUE;
    if (head == nullptr)
        return TRUE;

    HeadNode *coh = Copy(head);
    Literal *col = new Literal[2 * valnum];
    MemPos(coh, col, valnum);
    /* 向 原 CNF 结 构 体 尾 部 添 加 所 选 元 素 假 值 */
    for (trav = head; trav->next; trav = trav->next)
        ;

    trav->next = new HeadNode;
    trav->next->child = new Node;
    trav->next->next = nullptr;
    trav->next->num = 1;
    flag = TRUE;

    int data;
    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->next->child->data = -data; //  <0 假值
    trav->next->child->next = nullptr;

    Literal *p = new Literal;
    p->pos = trav->next->child;
    p->pre = nullptr;
    p->parent = trav->next;
    p->uncle = trav;
    p->next = ltr[2 * valnum - i - 1].next;
    ltr[2 * valnum - i - 1].next = p;
    if (Dpll(head, ltr, valnum, res, flag) == TRUE)
    {
        Destroy(coh, col, valnum);
        return TRUE;
    }

    /* 向 原 CNF 结 构 体 尾 部 添 加 所 选 元 素 真 值 */
    for (trav = coh; trav->next; trav = trav->next)
        ;

    trav->next = new HeadNode;
    trav->next->child = new Node;
    trav->next->next = nullptr;
    trav->next->num = 1;
    flag = TRUE;

    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->next->child->data = data; //  >0 真值
    trav->next->child->next = nullptr;

    Literal *p1 = new Literal;
    p1->pos = trav->next->child;
    p1->pre = nullptr;
    p1->parent = trav->next;
    p1->uncle = trav;
    p1->next = col[i].next;
    col[i].next = p1;
    int outcome = Dpll(coh, col, valnum, res, flag);
    return outcome;
}