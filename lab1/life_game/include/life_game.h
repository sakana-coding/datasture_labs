#ifndef LIFE_GAME_H
#define LIFE_GAME_H
#include <iostream>
#include <vector>
extern const int row;
extern const int col;
//打印矩阵
void PrintMatrix(const std::vector<std::vector<int>> &matrix);
//计算某个点的邻居个数
int CountNeighbors(int i, int j, const std::vector<std::vector<int>> &matrix);
//生成满足下一代的矩阵
void GenerateNextMatrix(std::vector<std::vector<int>> &matrix);

#endif