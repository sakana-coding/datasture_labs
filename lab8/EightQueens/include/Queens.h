#ifndef QUEENS_H
#define QUEENS_H

const int max_board = 30; // 棋盘最大尺寸
typedef unsigned __int128 CountType; // 用128位整数存储解的数量，防止大棋盘溢出

// 表示一个 N 皇后棋盘，逐行放置皇后
class Queens {
public:
    Queens(int size);
    bool is_solved() const;   // 是否已放满 board_size 个皇后
    void print() const;       // 打印当前棋盘
    bool unguarded(int col) const; // 当前行的 col 列是否不受攻击
    void insert(int col);     // 在当前行的 col 列放置皇后
    void remove(int col);     // 撤销当前行 col 列的皇后

    int board_size;

private:
    int count;                              // 已放置的皇后数（即当前行索引）
    bool queen_square[max_board][max_board]; // 棋盘格子占用状态
};

#endif
