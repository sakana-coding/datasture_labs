#include <iostream>
#include <vector>
#include "life_game.h"
using namespace std;
void PrintMatrix(const std::vector<std::vector<int>> &matrix)
{
    cout << "The current Life confinguration is :" << endl;
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (matrix[i][j] == 1)
                cout << '*';
            else
                cout << '-';
        }
        cout << endl;
    }
    cout << endl;
    cout << "continue viewing new generation?" << endl;
    cout << "(y , n)? ";
}
int CountNeighbors(int i, int j, const std::vector<std::vector<int>> &matrix)
{
    //在矩阵外面多加了两行两列所以可以把边角的格子统一起来运算
    int sum = 0;
    for (int k = i - 1; k <= i + 1; k++)
    {
        for (int l = j - 1; l <= j + 1; l++)
        {
            sum += matrix[k][l];
        }
    }
    sum -= matrix[i][j];
    return sum;
}
void GenerateNextMatrix(std::vector<std::vector<int>> &matrix)
{
    //这里需要一个临时矩阵来计算然后再覆盖回去，可以满足即时计算的要求
    vector<vector<int>> temp(row, vector<int>(col, 0));
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (CountNeighbors(i, j, matrix) == 3)
                temp[i][j] = 1;
            else if (CountNeighbors(i, j, matrix) == 2)
                temp[i][j] = matrix[i][j];
            else
                temp[i][j] = 0;
        }
    }
    matrix = temp;
}