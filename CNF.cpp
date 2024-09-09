#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

HeadNode *CreateClause(char *filename, int &valnum) // 读取文件并存储
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
    HeadNode *begin = new HeadNode;
    HeadNode *current = begin;
    for (int i = 0; i < ClauseNum; i++)
    {
        current->num = 0;
        current->child = new Node;

        Node *now = current->child;
        int temp;
        fin >> temp;
        if (temp == 0)
        {
            delete now;
            now = nullptr;
            continue;
        }
        now->data = temp;
        current->num++;

        fin >> temp;
        while (temp != 0)
        {
            now->next = new Node;
            now = now->next;
            now->data = temp;
            current->num++;
            fin >> temp;
        }
        now->next = nullptr;
        fin.get();
        if (i == ClauseNum - 1)
        {
            current->next = nullptr;
            continue;
        }
        current->next = new HeadNode;
        current = current->next;
    }
    fin.close();

    // current = begin; // 输出已存储数据
    // for (int i = 0; i < ClauseNum; i++)
    // {
    //     Node *ptr = current->child;
    //     cout << current->num << " : ";
    //     while (ptr)
    //     {
            
    //         cout << ptr->data << ' ';
    //         ptr = ptr->next;
    //     }
    //     cout << '0' << endl;
    //     current = current->next;
    // }
    return begin;
}
void MemPos(HeadNode *head, Literal *ltr, int valnum)
{
    for (int i = 0; i < 2*valnum; i++)
    {
        ltr[i].next = nullptr;
    }
    HeadNode *clausetrav = head;
    HeadNode *uncle = nullptr;
    Node *ltrtrav, *prev = nullptr;
    while(clausetrav)
    {
        ltrtrav = clausetrav->child;
        prev = nullptr;
        while(ltrtrav)
        {
            int n;
            if(ltrtrav->data < 0)
                n = ltrtrav->data + valnum;
            else
                n = ltrtrav->data + valnum - 1;
            Literal *p = new Literal;
            p->parent = clausetrav;
            p->uncle = uncle;
            p->pre = prev;
            p->pos = ltrtrav;

            p->next = ltr[n].next;
            ltr[n].next = p;// 头插法进入文字地址库

            prev = ltrtrav;
            ltrtrav = ltrtrav->next;
        }
        uncle = clausetrav;
        clausetrav = clausetrav->next;
    }
}