#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

/* 复 制 头 节 点 链 表 */
void CopyHeadNode(HeadNode *copy, HeadNode *stencil)
{
    HeadNode *ps = stencil;
    HeadNode *pc = copy;
    while (ps->next)
    {
        pc->num = ps->num;
        pc->next = new HeadNode;
        ps = ps->next;
        pc = pc->next;
    }
    pc->num = ps->num;
    pc->next = nullptr;
}
/* 复 制 子 节 点 链 表 */
void CopyNode(Node *copy, Node *stencil)
{
    Node *pc = copy;
    Node *ps = stencil;
    while (ps->next)
    {
        pc->data = ps->data;
        pc->next = new Node;
        ps = ps->next;
        pc = pc->next;
    }
    pc->data = ps->data;
    pc->next = nullptr;
}

/* 复 制 CNF 结 构 链 表 */
HeadNode *Copy(HeadNode *stencil)
{
    HeadNode *copy = new HeadNode;
    CopyHeadNode(copy, stencil);
    HeadNode *pc = copy;
    HeadNode *ps = stencil;
    while (pc)
    {
        pc->child = new Node;
        CopyNode(pc->child, ps->child);
        pc = pc->next;
        ps = ps->next;
    }
    return copy;
}

// /* 复 制 文 字 地 址 库 */
// void CopyLiteral(Literal *copy,Literal *ltr, int valnum)
// {
//     for (int i = 0; i < 2 * valnum; i++)
//     {
//         if (ltr[i].next == nullptr)
//         {
//             copy[i].next = nullptr;
//             continue;
//         }
//         copy[i].next = new Literal;
//         Literal *pc = copy[i].next;
//         Literal *ps = ltr[i].next;
//         while (ps->next)
//         {
//             pc->parent = ps->parent;
//             pc->pos = ps->pos;
//             pc->uncle = ps->uncle;
//             pc->pre = ps->pre;
//             pc->next = new Literal;
//             pc = pc->next;
//             ps = ps->next;
//         }
//         pc->parent = ps->parent;
//         pc->pos = ps->pos;
//         pc->uncle = ps->uncle;
//         pc->pre = ps->pre;
//         pc->next = nullptr;
//     }
// }