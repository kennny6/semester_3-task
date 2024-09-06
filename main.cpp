#include "global.h"

int main(void)
{
    cout << endl;
    cout << "\tWelcome to my application!" << endl;
    cout << "\tenter 1 for SAT, 2 for Sukodu:  " ;
    int choice;
    cin >> choice;
    int VALNUM;
    
    if(choice == 1)
    {
        char filename[] = "D:\\MyCode\\Code_c\\project_semester3\\SAT_sample\\baseline\\basefunc\\sat-20.cnf";
        HeadNode *head = CreateClause(filename,VALNUM);
        Literal *ltr = new Literal[VALNUM*2];
        MemPos(head,ltr,VALNUM);
        Dpll(head, ltr, VALNUM);
    }

    return 0;
}