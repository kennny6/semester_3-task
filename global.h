#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream>
#include <time.h>
#include <fstream>

using namespace std;

#define TRUE 1
#define FALSE 0
#define NORESULT 0
typedef int status;

// 结 构 体 定 义
typedef struct node
{
    int data;
    struct node *down; // 下一结点

    struct node *parent;// 连接头节点
    struct node *uncle;// 连接前一头节点
    struct node *pre;// 上一结点
    struct node *right;// 同变量的下一结点
    struct node *left;// 同变量的上一结点
} Node;

typedef Node HeadNode;

typedef struct literal
{
    int n;
    Node *next;
} Literal;

// 函 数 头
HeadNode *CreateClause(char *filename, int &valnum, Literal *&);

void DeleteNode(Literal *ltr, HeadNode *trash, int valnum);
status CheckSolo(HeadNode *head);
status SoloClause(HeadNode *&head, Literal *ltr, int valnum, int *, int &flag);

status Dpll(HeadNode *&head, Literal *ltr, int valnum, int *, int &flag);

HeadNode *Copy(HeadNode *stencil, Literal *&ltr, int valnum);

// void CopyLiteral(Literal *copy,Literal *ltr, int valnum);

void Destroy(HeadNode *head, Literal *ltr, int valnum);

#endif