#include <iostream>
#include "life_game.h"
#include "utilities.h"
#include <vector>
using namespace std;
const int row = 22;
const int col = 62;
void PrintPrompts()
{
    cout << '\t' << 't' << "welcome to my life game" << endl;
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
    while (true)
    {
        cin >> i >> j;
        if (!cin)
        {
            ClearBuffer();
            cout << "Invalid input. Please enter two integers: " << endl;
            continue;
        }
        if (i == -1 && j == -1)
            break;
        matrix[i][j] = 1;
    }
    PrintMatrix(matrix);
    while (true)
    {
        cin >> choice;
        if (!cin)
        {
            ClearBuffer();
            cout << "Invalid input. Please enter y or n: ";
            continue;
        }
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