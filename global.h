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
    struct node *next_node;

    struct node *parent;
    struct node *uncle;
    struct node *pre;
    struct node *next_same;
    struct node *prev_same;
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