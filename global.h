#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include <string>
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

/* ----------DPLL 系列函数---------- */ 
HeadNode *CreateClause(string filename, int &valnum, Literal *&);

void DeleteNode(Literal *ltr, HeadNode *trash, int valnum);
status CheckSolo(HeadNode *head);
status SoloClause(HeadNode *&head, Literal *ltr, int valnum, int *, int &flag);

status Dpll(HeadNode *&head, Literal *ltr, int valnum, int *, int &flag);

HeadNode *Copy(HeadNode *stencil, Literal *&ltr, int valnum);

void Destroy(HeadNode *head, Literal *ltr, int valnum);

/* ----------X-Sukodu 系列函数---------- */ 
status is_valid(int sukodu[][9], int x, int y);
status DFS(int sukodu[][9], int x, int y);
status Las_vegas(int sukodu[][9]);
void Sudoku_CNF(int a[][9]);
void showSudoku(const int board[][9]);
void Dig_holes(int sudoku[][9], int high_bond);
void Menu(int sudoku[][9], int answer[][9]);

#endif