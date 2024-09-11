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
        current->down = new Node;

        Node *now = current->down;
        Node *pre_node = nullptr;
        Node *sten_node_trav = stencil_trav->down;
        while (sten_node_trav->down)
        {
            now->data = sten_node_trav->data;
            now->parent = current;
            now->uncle = pre;
            now->pre = pre_node;
            now->down = new Node;

            int n;
            if (now->data < 0)
                n = now->data + valnum;
            else
                n = now->data + valnum - 1;
            if (ltr[n].next)
                ltr[n].next->left = now;
            now->right = ltr[n].next;
            now->left = nullptr;
            ltr[n].next = now;
            ltr[n].n++;

            sten_node_trav = sten_node_trav->down;// 向 后 迭 代 
            pre_node = now;
            now = now->down;
        }
        now->data = sten_node_trav->data;
        now->parent = current;
        now->uncle = pre;
        now->pre = pre_node;
        now->down = nullptr;// 末位置空
        
        int n;
        if (now->data < 0)
            n = now->data + valnum;
        else
            n = now->data + valnum - 1;
        if (ltr[n].next)
            ltr[n].next->left = now;
        now->right = ltr[n].next;
        now->left = nullptr;
        ltr[n].next = now;
        ltr[n].n++;

        stencil_trav = stencil_trav->right;
        current->right = new Node;
        pre = current;
        current = current->right;
    }
    delete current;
    pre->right = nullptr;

    current = begin; // 输出已存储数据
    cout << "-----------------------" << endl;
    while (current)
    {
        Node *ptr = current->down;
        cout << current->data << " : ";
        while (ptr)
        {

            cout << ptr->data << ' ';
            ptr = ptr->down;
        }
        cout << '0' << endl;
        current = current->right;
    }
    cout << "-----------------------" << endl;
    for (int i = 0; i < 2 * valnum; i++)
    {
        Node *ptr = ltr[i].next;
        cout << ltr[i].n << ": ";
        while (ptr)
        {
            cout << ptr->data << ' ';
            ptr = ptr->right;
        }
        cout << endl;
    }
    return begin;
}
