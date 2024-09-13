#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"


int main(void)
{
    clock_t StartTime, EndTime;
    cout << endl;
    cout << "\tWelcome to my application!" << endl;
    printf("-------------------------------------------------\n");
    cout << "\tEnter 1 for SAT, 2 for Sukodu:  ";
    int choice;
    cin >> choice;
    int VALNUM;

    if (choice == 1)
    {
        string filename;
        cout << "\tEnter the filename you wanted: ";
        cin >> filename;
        string extension = ".res";
        Literal *ltr;
        HeadNode *head = CreateClause(filename, VALNUM, ltr);
        int flag = CheckSolo(head);

        int *result = new int[VALNUM];
        for (int i = 0; i < VALNUM; i++)
            result[i] = 0;

        StartTime = clock();
        int outcome = Dpll(head, ltr, VALNUM, result, flag);
        EndTime = clock();

        ofstream fout(filename + extension);
        if (outcome == FALSE)
        {
            fout << "s -1" << endl;
            fout << "T " << (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
        }
        else
        {

            fout << "s 1" << endl;
            fout << "v ";
            for (int i = 0; i < VALNUM; i++)
            {
                if (result[i] == -1)
                    fout << -(i + 1) << ' ';
                else if (result[i] == 1)
                    fout << i + 1 << ' ';
                else
                    fout << 'u' << i + 1 << ' ';
            }
            fout << endl;
            fout << "t " << (double)(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
        }
    }
    else if (choice == 2)
    {
        srand((unsigned)time(0));
        printf("-------------------------------------------------\n");
        cout << "\tChoose the difficulty for your x-sudoku: " << endl;
        cout << "1.Easy" << "\t2.Medium" << "\t3.Difficult" << endl;
        cout << "\tEnter your choice: ";
        int level;
        cin >> level;
        cout << "Generating....." << endl;
        // 生 成 数 独 终 局
        int sudoku[9][9];
        int outcome;
        do
        {
            outcome = Las_vegas(sudoku);
        } while (outcome == FALSE);
        showSudoku(sudoku);
        int answer[9][9]; // 备份答案
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                answer[i][j] = sudoku[i][j];
        switch (level)
        {
        case 1:
            Dig_holes(sudoku, 4);
            break;
        case 2:
            Dig_holes(sudoku, 5);
            break;
        case 3:
            Dig_holes(sudoku, 6);
            break;
        }
        Menu(sudoku, answer);
    }

    return 0;
}