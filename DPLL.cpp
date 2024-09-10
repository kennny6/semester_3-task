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
    int n = 0, max = 0, i;
    while (n < 2 * valnum)
    {
        if (max < ltr[n].n)
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

    Literal *col = new Literal[2 * valnum];
    for (int i = 0; i < 2 * valnum; i++)
    {
        col[i].n = 0;
        col[i].next = nullptr;
    }
    HeadNode *coh = Copy(head, col, valnum);
    /* 向 原 CNF 结 构 体 尾 部 添 加 所 选 元 素 真 值 */
    for (trav = head; trav->next_same; trav = trav->next_same)
        ;

    trav->next_same = new HeadNode;
    trav->next_same->next_node = new Node;
    trav->next_same->next_same = nullptr;
    trav->next_same->data = 1;
    flag = TRUE;

    int data;
    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->next_same->next_node->data = -data; //  >0 真值
    trav->next_same->next_node->next_node = nullptr;

    Node *now = trav->next_same->next_node;
    now->parent = trav->next_same;
    now->uncle = trav;
    now->pre = nullptr;

    ltr[2 * valnum - i - 1].next->prev_same = now;
    now->next_same = ltr[2 * valnum - i - 1].next;
    now->prev_same = nullptr;
    ltr[2 * valnum - i - 1].next = now;
    ltr[2 * valnum - i - 1].n++;

    if (Dpll(head, ltr, valnum, res, flag) == TRUE)
    {
        Destroy(coh, col, valnum);
        return TRUE;
    }

    /* 向 副 本 CNF 结 构 体 尾 部 添 加 所 选 元 素 假 值 */
    for (trav = coh; trav->next_same; trav = trav->next_same)
        ;

    trav->next_same = new HeadNode;
    trav->next_same->next_node = new Node;
    trav->next_same->next_same = nullptr;
    trav->next_same->data = 1;
    flag = TRUE;

    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->next_same->next_node->data = data; //  >0 真值
    trav->next_same->next_node->next_node = nullptr;

    now = trav->next_same->next_node;
    now->parent = trav->next_same;
    now->uncle = trav;
    now->pre = nullptr;
    
    ltr[i].next->prev_same = now;
    now->next_same = ltr[i].next;
    now->prev_same = nullptr;
    ltr[i].next = now;
    ltr[i].n++;

    int outcome = Dpll(coh, col, valnum, res, flag);
    return outcome;
}