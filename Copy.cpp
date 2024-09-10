#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

/* 复 制 头 节 点 链 表 */
HeadNode *Copy(HeadNode *stencil, Literal *&ltr, int valnum)
{
    HeadNode *begin = new HeadNode; // 副本
    HeadNode *stencil_trav = stencil;

    HeadNode *current = begin;
    HeadNode *pre = nullptr;
    while (stencil_trav)
    {
//------------------------------复制子节点----------------------------//
        current->data = stencil_trav->data;
        current->next_node = new Node;

        Node *now = current->next_node;
        Node *pre_node = nullptr;
        Node *sten_node_trav = stencil_trav->next_node;
        while (sten_node_trav->next_node)
        {
            now->data = sten_node_trav->data;
            now->parent = current;
            now->uncle = pre;
            now->pre = pre_node;
            now->next_node = new Node;

            int n;
            if (now->data < 0)
                n = now->data + valnum;
            else
                n = now->data + valnum - 1;
            if (ltr[n].next)
                ltr[n].next->prev_same = now;
            now->next_same = ltr[n].next;
            now->prev_same = nullptr;
            ltr[n].next = now;
            ltr[n].n++;
            current->data++;

            sten_node_trav = sten_node_trav->next_node;// 向 后 迭 代 
            pre_node = now;
            now = now->next_node;
        }
        now->data = sten_node_trav->data;
        now->parent = current;
        now->uncle = pre;
        now->pre = pre_node;
        now->next_node = nullptr;// 末位置空
        
        int n;
        if (now->data < 0)
            n = now->data + valnum;
        else
            n = now->data + valnum - 1;
        if (ltr[n].next)
            ltr[n].next->prev_same = now;
        now->next_same = ltr[n].next;
        now->prev_same = nullptr;
        ltr[n].next = now;
        ltr[n].n++;
        current->data++;

        stencil_trav = stencil_trav->next_same;
        current->next_same = new Node;
        pre = current;
        current = current->next_same;
    }
    delete current;
    pre->next_same = nullptr;

    return begin;
}
