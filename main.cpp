#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

int main(void)
{
    clock_t StartTime,EndTime;
    cout << endl;
    cout << "\tWelcome to my application!" << endl;
    cout << "\tenter 1 for SAT, 2 for Sukodu:  " ;
    int choice;
    cin >> choice;
    int VALNUM;
    
    if(choice == 1)
    {
        string filename;
        cout << "\tEnter the filename you wanted: ";
        cin >> filename;
        string ext = ".cnf";
        string ext2 = ".res";
        HeadNode *head = CreateClause(filename + ext,VALNUM);
        int flag = CheckSolo(head);

        Literal *ltr = new Literal[VALNUM*2];
        int *result = new int[VALNUM];
        for(int i = 0; i < VALNUM; i++)
            result[i] = 0;
        MemPos(head,ltr,VALNUM);

        StartTime = clock();
        int outcome = Dpll(head, ltr, VALNUM, result, flag);
        EndTime = clock();

        ofstream fout(filename + ext2);
        if(outcome == FALSE)
        {
            fout << "The result is : 0" << endl;
            fout<<"T "<<(double)(EndTime-StartTime)/CLOCKS_PER_SEC*1000.0<<" ms\n";
        }
        else
        {
            
            fout << "The result is : 1" << endl;
            for(int i = 0; i < VALNUM; i++)
            {
                if(result[i] == -1)
                    fout << -(i+1) << ' ';
                else if(result[i] == 1)
                    fout << i+1 << ' ';
                else
                    fout << 'u' << i+1 << ' ';
            }
            fout << endl;
            fout<<"T "<<(double)(EndTime-StartTime)/CLOCKS_PER_SEC*1000.0<<" ms\n";
        }
    }
    
    return 0;
}