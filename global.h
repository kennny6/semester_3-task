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
    struct node *next;
}Node;

typedef struct head
{
    int num;
    struct head *next;
    Node *child;
}HeadNode;

typedef struct literal
{
    HeadNode *parent;
    HeadNode *uncle;
    Node *pos;
    Node *pre;
    struct literal *next;
}Literal;


// 函 数 头
HeadNode *CreateClause(char *filename, int &valnum);
void MemPos(HeadNode *, Literal *, int);

void DeleteNode(Literal *ltr, HeadNode *trash, int valnum);
status CheckSolo(HeadNode *head);
status SoloClause(HeadNode *&head, Literal *ltr, int valnum, int *, int &flag);

status Dpll(HeadNode *&head, Literal *ltr, int valnum, int *, int &flag);

void CopyHeadNode(HeadNode *copy, HeadNode *stencil);
void CopyNode(Node *copy, Node *stencil);
HeadNode *Copy(HeadNode *stencil);


// void CopyLiteral(Literal *copy,Literal *ltr, int valnum);

void Destroy(HeadNode *head, Literal *ltr, int valnum);

#endif