#include "global.h"

status is_valid(int sudoku[][9], int x, int y)
{
    for (int i = 0; i < 9; i++) // 行检查
    {
        if (y == i)
            continue;
        if (sudoku[x][y] == sudoku[x][i])
            return FALSE;
    }
    for (int i = 0; i < 9; i++) // 列检查
    {
        if (x == i)
            continue;
        if (sudoku[x][y] == sudoku[i][y])
            return FALSE;
    }
    // 宫检查
    int m = x / 3;
    int n = y / 3;
    for (int i = m * 3; i < m * 3 + 3; i++)
        for (int j = n * 3; j < n * 3 + 3; j++)
        {
            if (i == x && j == y)
                continue;
            if (sudoku[x][y] == sudoku[i][j])
                return FALSE;
        }

    // 对角线检查
    if (x == y)
    {
        for (int i = 0; i < 9; i++)
        {
            if (i == x)
                continue;
            if (sudoku[x][y] == sudoku[i][i])
                return FALSE;
        }
    }
    if (x + y == 8)
    {
        for (int i = 0; i < 9; i++)
        {
            if (i == x)
                continue;
            if (sudoku[x][y] == sudoku[i][8 - i])
                return FALSE;
        }
    }
    return TRUE;
}
status DFS(int sudoku[][9], int x, int y)
{
    int i, flag = 0;
    if (x > 8)
        return TRUE;
    if (sudoku[x][y])
    {
        if (y + 1 < 9)
            return DFS(sudoku, x, y + 1);
        else
            return DFS(sudoku, x + 1, 0);
    }
    for (i = 1; i < 10 && flag == 0; i++)
    {
        sudoku[x][y] = i;
        if (is_valid(sudoku, x, y))
        {
            if (y + 1 < 9)
                flag = DFS(sudoku, x, y + 1);
            else
                flag = DFS(sudoku, x + 1, 0);
        }
    }
    if (i > 9 && flag == 0)
        sudoku[x][y] = 0;
    return flag;
}
void Rand_pos(int &x, int &y)
{
    x = rand() % 9;
    y = rand() % 9;
}
status Las_vegas(int sudoku[][9])
{
    int val;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            sudoku[i][j] = 0;
    for (int i = 0; i < 11; i++)
    {
        int x, y;
        do
            Rand_pos(x, y);
        while (sudoku[x][y] != 0);
        do
        {
            bool c[10] = {false};
            val = rand() % 9 + 1;
            sudoku[x][y] = val;
            c[val] = true;
            if (c[1] && c[2] && c[3] && c[4] && c[5] && c[6] && c[7] && c[8] && c[9])
            {
                i--;
                sudoku[x][y] = 0;
                break;
            }
        } while (is_valid(sudoku, x, y) == FALSE);
    }
    return DFS(sudoku, 0, 0);
}
status is_only(int sudoku[][9], int x, int y)
{
    int copy[9][9];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            copy[i][j] = sudoku[i][j];
    for (int i = 1; i < 10; i++)
    {
        if (i == sudoku[x][y])
            continue;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                copy[i][j] = sudoku[i][j];
        do
        {
            if (i != sudoku[x][y])
                copy[x][y] = i++;
            else
                i++;
        } while (i < 10 && is_valid(copy, x, y) == FALSE);
        if (i == 10)
            return true;
        if (DFS(copy, 0, 0) == TRUE)
            return FALSE;
    }
    return TRUE;
}
void Dig_holes(int sudoku[][9], int high_bond)
{
    int flag[9][9];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            flag[i][j] = 0;

    int sum = 0, line_hole_num[9], line_check_num[9];
    for (int i = 0; i < 9; i++)
    {
        line_hole_num[i] = 0;
        line_check_num[i] = 0;
    }

    for (int i = 0; i < 9; i++)
    {
        while (line_hole_num[i] < high_bond)
        {
            int y;
            do
                y = rand() % 9;
            while (flag[i][y] == 1);
            flag[i][y] = 1;
            line_check_num[i]++;
            if (is_only(sudoku, i, y) == TRUE)
            {
                line_hole_num[i]++;
                sudoku[i][y] = 0;
                // showSudoku(sudoku);
            }
            if (line_check_num[i] > 9)
                continue;
        }
    }
}
void Sudoku_CNF(int sudoku[][9])
{
    ofstream cnf_file("xsudoku.cnf");

    if (!cnf_file)
    {
        printf("can't open!\n");
    }

    int known_num = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (sudoku[i][j] != 0)
                known_num++;

    cnf_file << "p" << " " << "cnf" << " " << 999 << " " << 9738 + known_num << endl;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (sudoku[i][j] != 0)
                cnf_file << i << j << sudoku[i][j] << ' ' << 0 << endl;

    // 格 约 束 --- 每格至少有一个1-9
    for (int i = 1; i < 10; i++)
        for (int j = 1; j < 10; j++)
        {
            for (int n = 1; n < 10; n++)
                cnf_file << i << j << n << ' ';
            cnf_file << 0 << endl;
        }
    // 格 约 束 --- 每格只有一个数字
    for (int i = 1; i < 10; i++)
        for (int j = 1; j < 10; j++)
            for (int m = 9; m > 0; m--)
                for (int n = 1; n < m; n++)
                    cnf_file << -(i * 100 + j * 10 + n) << ' ' << -(i * 100 + j * 10 + m) << ' ' << 0 << endl;

    // 行 约 束 --- 每行至少有一个1-9
    for (int i = 1; i < 10; i++)
        for (int n = 1; n < 10; n++)
        {
            for (int j = 1; j < 10; j++)
                cnf_file << i * 100 + j * 10 + n << ' ';
            cnf_file << 0 << endl;
        }
    // 行 约 束 --- 每行的数字不冲突
    for (int i = 1; i < 10; i++)            // 行
        for (int n = 1; n < 10; n++)        // 数
            for (int x = 9; x > 0; x--)     // 大列号
                for (int y = 1; y < x; y++) // 小列号
                    cnf_file << -(i * 100 + y * 10 + n) << ' ' << -(i * 100 + x * 10 + n) << ' ' << 0 << endl;

    // 列 约 束 --- 每列至少有一个1-9
    for (int i = 1; i < 10; i++)
        for (int n = 1; n < 10; n++)
        {
            for (int j = 1; j < 10; j++)
                cnf_file << j * 100 + i * 10 + n << ' ';
            cnf_file << 0 << endl;
        }
    // 列 约 束 --- 每列的数字不冲突
    for (int i = 1; i < 10; i++)            // 列
        for (int n = 1; n < 10; n++)        // 数
            for (int x = 9; x > 0; x--)     // 大行号
                for (int y = 1; y < x; y++) // 小行号
                    cnf_file << -(y * 100 + i * 10 + n) << ' ' << -(x * 100 + i * 10 + n) << ' ' << 0 << endl;

    // 宫 约 束 --- 含有一套1-9
    for (int p = 0; p <= 2; ++p)
        for (int q = 0; q <= 2; ++q)
            for (int k = 1; k <= 9; ++k)
            {
                for (int i = 1; i <= 3; ++i)
                {
                    for (int j = 1; j <= 3; ++j)
                    {
                        int x = 3 * p + i;
                        int y = 3 * q + j;
                        // fprintf(fp, "%d%d%d ", x, y, k);
                        cnf_file << x << y << k << ' ';
                    }
                }
                // fprintf(fp, "0\n");
                cnf_file << 0 << endl;
            }

    // 对角线约束 --- 对角线上含有一套1-9
    for (int k = 1; k <= 9; ++k)
    {
        for (int i = 1; i <= 9; ++i)
            cnf_file << i << i << k << ' ';

        cnf_file << 0 << endl;
    }
    for (int k = 1; k <= 9; ++k)
    {
        for (int i = 1; i <= 9; ++i)
            cnf_file << i << (10 - i) << k << ' ';

        cnf_file << 0 << endl;
    }
    // 对角线约束 --- 对角线上文字不重复
    for (int k = 1; k <= 9; ++k)
        for (int i = 1; i <= 8; ++i)
            for (int i_ = i + 1; i_ <= 9; ++i_)

                cnf_file << -(i * 100 + i * 10 + k) << ' ' << -(i_ * 100 + i_ * 10 + k) << ' ' << 0 << endl;
    for (int k = 1; k <= 9; ++k)
        for (int i = 1; i <= 8; ++i)
            for (int i_ = i + 1; i_ <= 9; ++i_)
                cnf_file << -(i * 100 + (10 - i) * 10 + k) << ' ' << -(i_ * 100 + (10 - i_) * 10 + k) << ' ' << 0 << endl;

    cnf_file.close();
}
void showSudoku(const int board[][9])
{
    for (int j = 1; j <= 39; ++j)
        cout << "-";
    cout << endl;
    for (int i = 0; i <= 8; ++i)
    {
        printf(" | ");
        for (int j = 0; j <= 8; ++j)
        {
            printf(" %d ", board[i][j]);
            if ((j + 1) % 3 == 0)
                printf(" | ");
        }
        if ((i + 1) % 3 == 0)
        {
            printf("\n");
            for (int j = 1; j <= 39; ++j)
                printf("-");
        }
        printf("\n");
    }
}
void Menu(int sudoku[][9], int answer[][9])
{
    system("cls");
    showSudoku(sudoku);

    printf("\n\n");
    printf("               Menu for X-Sudoku \n");
    printf("-------------------------------------------------\n");
    printf("\t1.Start\t2.Answer\t0.Exit\n");
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        int elem;
        cout << "Enter ijn for sudoku[i+1][j+1] = n (Enter 2 for answer) : ";
        cin >> elem;
        while (elem != 2)
        {
            int x = elem / 100;
            int y = (elem % 100) / 10;
            int n = elem % 10;
            printf("-------------------------------------------------\n");
            if (answer[x - 1][y - 1] == n)
            {
                sudoku[x - 1][y - 1] = n;
                cout << "correct!" << endl;
                showSudoku(sudoku);
                printf("-------------------------------------------------\n");
                cout << "Enter ijn for sudoku[i+1][j+1] = n (Enter 2 for answer) : ";
                cin >> elem;
            }
            else
            {
                cout << "Wrong Answer!  Try again!" << endl;
                cout << "Enter ijn for sudoku[i+1][j+1] = n (Enter 2 for answer) : ";
                cin >> elem;
            }
        }
        choice = 2;
    }
    if (choice == 2)
    {
        Sudoku_CNF(sudoku);
        int VALNUM;
        string filename = "xsudoku";
        Literal *ltr;
        HeadNode *head = CreateClause(filename, VALNUM, ltr);
        int flag = CheckSolo(head);

        int *result = new int[VALNUM];
        for (int i = 0; i < VALNUM; i++)
            result[i] = 0;

        int outcome = Dpll(head, ltr, VALNUM, result, flag);
        int x, y, n;
        
        showSudoku(answer);
        getchar();
        getchar();
        return;
    }
    if (choice == 0)
        return;
}