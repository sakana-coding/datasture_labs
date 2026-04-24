#include "Queens.h"

#include <iostream>
#include <stdexcept>

Queens::Queens(int size) {
    if (size <= 0 || size > max_board) {
        throw std::invalid_argument("board size is out of range");
    }

    board_size = size;
    count = 0;

    // 初始化棋盘，所有格子置为空
    for (int i = 0; i < max_board; ++i) {
        for (int j = 0; j < max_board; ++j) {
            queen_square[i][j] = false;
        }
    }
}

bool Queens::is_solved() const {
    return count == board_size;
}

bool Queens::unguarded(int col) const {
    if (col < 0 || col >= board_size) {
        return false;
    }
    if (count < 0 || count >= board_size) {
        return false;
    }

    // 检查同列是否有皇后
    for (int row = 0; row < count; ++row) {
        if (queen_square[row][col]) {
            return false;
        }
    }

    // 检查两条对角线是否有皇后
    int row = count - 1;
    int left = col - 1;
    int right = col + 1;
    while (row >= 0) {
        if (left >= 0 && queen_square[row][left]) {
            return false;
        }
        if (right < board_size && queen_square[row][right]) {
            return false;
        }
        --row;
        --left;
        ++right;
    }

    return true;
}

void Queens::insert(int col) {
    if (col < 0 || col >= board_size) {
        return;
    }
    if (count < 0 || count >= board_size) {
        return;
    }

    // 在第 count 行的 col 列放置皇后，count 即为当前待放行
    queen_square[count][col] = true;
    ++count;
}

void Queens::remove(int col) {
    if (col < 0 || col >= board_size) {
        return;
    }
    if (count <= 0) {
        return;
    }

    queen_square[count - 1][col] = false;
    --count;
}

void Queens::print() const {
    for (int i = 0; i < board_size; ++i) {
        for (int j = 0; j < board_size; ++j) {
            if (queen_square[i][j]) {
                std::cout << "Q ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << '\n';
    }
}
