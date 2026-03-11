#include <iostream>
#include "life_game.h"
#include "utilities.h"
#include <vector>
using namespace std;
const int row = 22;
const int col = 62;
//写提示词
void PrintPrompts()
{
    cout << '\t' << 't' << "welcome to my life game." << endl;
    cout << "This game uses a grid of size 20 by 60 in which cell can either be occupied by an organism or not. " << endl;
    cout << "The occupied cells change from generation to generation" << endl;
    cout << "you have to input some life seeds to start game" << endl;
    cout << "Terminate the list with the special pair -1 -1" << endl;
}

int main()
{
    vector<vector<int>> matrix;
    Initialize(row, col, matrix);
    PrintPrompts();
    char choice;
    int i;
    int j;
    // 主循环
    while (true)
    {
        cin >> i >> j;
        // 保持鲁棒性
        if (!cin)
        {
            ClearBuffer();
            cout << "Invalid input. Please enter two integers that first bigger than 0 and not bigger than 20 ans second bigger than 0 and not bigger than 60" << endl;
            continue;
        }
        if (i == -1 && j == -1)
            break;
        if (i >= 1 && i <= 20 && j >= 1 && j <= 60)
            matrix[i][j] = 1;
        else
        {
            ClearBuffer();
            cout << "Invalid input. Please enter two integers that first bigger than 0 and not bigger than 20 ans second bigger than 0 and not bigger than 60" << endl;
            continue;
        }
    }
    PrintMatrix(matrix);
    while (true)
    {
        cin >> choice;
        // 保持鲁棒性
        if (!cin)
        {
            ClearBuffer();
            cout << "Invalid input. Please enter y or n: ";
            continue;
        }
        // 选择是否继续输出下一代
        if (choice == 'Y' || choice == 'y')
        {
            GenerateNextMatrix(matrix);
            PrintMatrix(matrix);
        }
        else if (choice == 'N' || choice == 'n')
        {
            break;
        }
        else
        {
            ClearBuffer();
            cout << "Invalid input. Please enter y or n: " << endl;
        }
    }
}