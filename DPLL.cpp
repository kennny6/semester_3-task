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
    for (trav = head; trav->right; trav = trav->right)
        ;

    trav->right = new HeadNode;
    trav->right->down = new Node;
    trav->right->right = nullptr;
    trav->right->data = 1;
    flag = TRUE;

    int data;
    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->right->down->data = -data; //  >0 真值
    trav->right->down->down = nullptr;

    Node *now = trav->right->down;
    now->parent = trav->right;
    now->uncle = trav;
    now->pre = nullptr;

    ltr[2 * valnum - i - 1].next->left = now;
    now->right = ltr[2 * valnum - i - 1].next;
    now->left = nullptr;
    ltr[2 * valnum - i - 1].next = now;
    ltr[2 * valnum - i - 1].n++;

    if (Dpll(head, ltr, valnum, res, flag) == TRUE)
    {
        Destroy(coh, col, valnum);
        return TRUE;
    }

    /* 向 副 本 CNF 结 构 体 尾 部 添 加 所 选 元 素 假 值 */
    for (trav = coh; trav->right; trav = trav->right)
        ;

    trav->right = new HeadNode;
    trav->right->down = new Node;
    trav->right->right = nullptr;
    trav->right->data = 1;
    flag = TRUE;

    if (i < valnum)
        data = i - valnum;
    else
        data = i + 1 - valnum;
    trav->right->down->data = data; //  >0 真值
    trav->right->down->down = nullptr;

    now = trav->right->down;
    now->parent = trav->right;
    now->uncle = trav;
    now->pre = nullptr;
    
    col[i].next->left = now;
    now->right = col[i].next;
    now->left = nullptr;
    col[i].next = now;
    col[i].n++;

    int outcome = Dpll(coh, col, valnum, res, flag);
    return outcome;
}