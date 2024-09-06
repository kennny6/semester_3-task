#ifndef _GLOBAL_H_
#define _GLOBAL_H_

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
status MemPos(HeadNode *, Literal *, int);
void DeleteNode(Literal *ltr, HeadNode *trash, int valnum);
status CheckSolo(HeadNode *head);
status SoloClause(HeadNode *head, Literal *ltr, int valnum);
status Dpll(HeadNode *head, Literal *ltr, int valnum);
void CopyHeadNode(HeadNode *copy, HeadNode *stencil);
void CopyNode(Node *copy, Node *stencil);
HeadNode *Copy(HeadNode *stencil);
void CopyLiteralPos(Literal *copy, Literal *stencil);
Literal *Copy(Literal *ltr, int valnum);
void Destroy(HeadNode *head, Literal *ltr, int valnum);

#endif