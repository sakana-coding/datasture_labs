#ifndef LIFE_GAME_H
#define LIFE_GAME_H
#include<iostream>
#include<vector>
extern const int row;
extern const int col;
void PrintMatrix(const std::vector<std::vector<int>>& matrix);
int CountNeighbors(int i , int j , const std::vector<std::vector<int>>& matrix);
void GenerateNextMatrix(std::vector<std::vector<int>> &matrix);

#endif