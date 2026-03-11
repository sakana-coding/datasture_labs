#include <iostream>
#include "utilities.h"
#include <vector>
#include <limits>
using namespace std;
void Initialize(int row, int col, vector<vector<int>> &matrix)
{
    vector<vector<int>> vec(row, vector<int>(col, 0));
    matrix = vec;
}
void ClearBuffer()
{

        cin.clear();
        cin.ignore(INT_MAX, '\n');
    
}