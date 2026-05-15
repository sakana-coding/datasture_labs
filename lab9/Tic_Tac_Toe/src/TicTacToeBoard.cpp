#include "TicTacToeBoard.h"

#include <iostream>

const int TicTacToeBoard::DEFAULT_SIZE;
const int TicTacToeBoard::MIN_SIZE;
const int TicTacToeBoard::MAX_SIZE;

TicTacToeBoard::TicTacToeBoard(int size) {
    reset(size);
}

void TicTacToeBoard::reset(int size) {
    // 防御式处理：即使调用者传入非法大小，也强制改回默认 3x3。
    if (size < MIN_SIZE || size > MAX_SIZE) {
        size = DEFAULT_SIZE;
    }

    size_ = size;
    board_.assign(size_, std::vector<int>(size_, EMPTY));
}

int TicTacToeBoard::size() const {
    return size_;
}

bool TicTacToeBoard::isInside(int x, int y) const {
    return x >= 0 && x < size_ && y >= 0 && y < size_;
}

bool TicTacToeBoard::isEmpty(int x, int y) const {
    return isInside(x, y) && board_[x][y] == EMPTY;
}

int TicTacToeBoard::getCell(int x, int y) const {
    if (!isInside(x, y)) {
        return EMPTY;
    }
    return board_[x][y];
}

bool TicTacToeBoard::place(int x, int y, int role) {
    if (!isEmpty(x, y)) {
        return false;
    }
    board_[x][y] = role;
    return true;
}

void TicTacToeBoard::undo(int x, int y) {
    if (isInside(x, y)) {
        board_[x][y] = EMPTY;
    }
}

bool TicTacToeBoard::hasWon(int role) const {
    // 对 N x N 棋盘，胜利条件为同一角色连成 N 个：
    // 任意一整行、任意一整列、两条主对角线之一。
    for (int i = 0; i < size_; ++i) {
        bool rowWin = true;
        bool colWin = true;

        for (int j = 0; j < size_; ++j) {
            if (board_[i][j] != role) {
                rowWin = false;
            }
            if (board_[j][i] != role) {
                colWin = false;
            }
        }

        if (rowWin || colWin) {
            return true;
        }
    }

    bool mainDiagonalWin = true;
    bool antiDiagonalWin = true;
    for (int i = 0; i < size_; ++i) {
        if (board_[i][i] != role) {
            mainDiagonalWin = false;
        }
        if (board_[i][size_ - 1 - i] != role) {
            antiDiagonalWin = false;
        }
    }

    return mainDiagonalWin || antiDiagonalWin;
}

bool TicTacToeBoard::isFull() const {
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j) {
            if (board_[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

void TicTacToeBoard::print() const {
    for (int i = 0; i < size_; ++i) {
        std::cout << " ";
        for (int j = 0; j < size_; ++j) {
            std::cout << board_[i][j];
            if (j + 1 < size_) {
                std::cout << " ";
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::vector<std::pair<int, int> > TicTacToeBoard::emptyCells() const {
    std::vector<std::pair<int, int> > cells;
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j) {
            if (board_[i][j] == EMPTY) {
                cells.push_back(std::make_pair(i, j));
            }
        }
    }
    return cells;
}
