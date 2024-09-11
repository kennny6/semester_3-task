#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

HeadNode *CreateClause(char *filename, int &valnum, Literal *&ltr) // 读取文件并存储
{
    ifstream fin(filename);
    if (!fin)
    {
        cout << "Fail to open the file." << endl;
        exit(0);
    }
    char flag;
    char trash[100];
    fin >> flag;
    while (flag != 'p') // 处理注释等无关内容
    {
        fin.getline(trash, 100);
        fin >> flag;
    }

    /* 对文件内容进行读取 */
    int ValNum, ClauseNum;
    fin >> trash >> ValNum >> ClauseNum;
    valnum = ValNum;
    ltr = new Literal[valnum * 2];
    for (int i = 0; i < 2 * valnum; i++)
    {
        ltr[i].n = 0;
        ltr[i].next = nullptr;
    }

    HeadNode *begin = new HeadNode;
    HeadNode *current = begin;
    HeadNode *pre = nullptr;
    for (int i = 0; i < ClauseNum; i++)
    {
        current->data = 0;
        current->down = new Node;

        Node *now = current->down;
        Node *pre_node = nullptr;
        int temp;
        fin >> temp;
        if (temp == 0)
        {
            delete now;
            now = nullptr;
            continue;
        }

        int n;
        if (temp < 0)
            n = temp + valnum;
        else
            n = temp + valnum - 1;
        now->data = temp;
        now->parent = current;
        now->uncle = pre;
        now->pre = pre_node;

        if (ltr[n].next)
            ltr[n].next->left = now;
        now->right = ltr[n].next;
        now->left = nullptr;
        ltr[n].next = now;
        ltr[n].n++;
        current->data++;

        fin >> temp;
        while (temp != 0)
        {
            now->down = new Node;
            pre_node = now;
            now = now->down;

            now->data = temp;
            int n;
            if (temp < 0)
                n = temp + valnum;
            else
                n = temp + valnum - 1;
            now->parent = current;
            now->uncle = pre;
            now->pre = pre_node;

            if (ltr[n].next)
                ltr[n].next->left = now;
            now->right = ltr[n].next;
            now->left = nullptr;
            ltr[n].next = now;
            ltr[n].n++;
            current->data++;

            fin >> temp;
        }
        now->down = nullptr;
        fin.get();
        if (i == ClauseNum - 1)
        {
            current->right = nullptr;
            continue;
        }
        current->right = new HeadNode;
        pre = current;
        current = current->right;
    }
    fin.close();

    // current = begin; // 输出已存储数据
    // for (int i = 0; i < ClauseNum; i++)
    // {
    //     Node *ptr = current->down;
    //     cout << current->data << " : ";
    //     while (ptr)
    //     {

    //         cout << ptr->data << ' ';
    //         ptr = ptr->down;
    //     }
    //     cout << '0' << endl;
    //     current = current->right;
    // }
    // cout << "-----------------------" << endl;
    // for (int i = 0; i < 2 * valnum; i++)
    // {
    //     Node *ptr = ltr[i].next;
    //     cout << ltr[i].n << ": ";
    //     while (ptr)
    //     {
    //         cout << ptr->data << ' ';
    //         ptr = ptr->right;
    //     }
    //     cout << endl;
    //}
    return begin;
}