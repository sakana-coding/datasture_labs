#ifndef TIC_TAC_TOE_BOARD_H
#define TIC_TAC_TOE_BOARD_H

#include <utility>
#include <vector>

class TicTacToeBoard {
public:
    static const int DEFAULT_SIZE = 3;
    static const int MIN_SIZE = 3;
    static const int MAX_SIZE = 9;

    // 用枚举保存棋盘状态，避免直接在代码里写魔法数字。
    enum Cell {
        EMPTY = 0,
        COMPUTER = 1,
        PLAYER = 2
    };

    /**
     * 井字棋棋盘类：
     * 0 表示空位，1 表示电脑，2 表示玩家。
     * 坐标规则：X 表示行号，Y 表示列号，范围都是 0~size-1。
     */
    explicit TicTacToeBoard(int size = DEFAULT_SIZE);

    // 重新设置棋盘大小，并清空棋盘。
    void reset(int size);

    // 当前棋盘边长。
    int size() const;

    // 判断坐标是否在棋盘范围内。
    bool isInside(int x, int y) const;

    // 判断某个位置是否为空。
    bool isEmpty(int x, int y) const;

    // 获取某个格子的值；越界时返回 EMPTY，方便图形界面安全绘制。
    int getCell(int x, int y) const;

    // 落子。成功返回 true；越界或位置已被占用则返回 false。
    bool place(int x, int y, int role);

    // 撤销落子，用于电脑试探“能不能赢/需不需要防守”。
    void undo(int x, int y);

    // 判断指定角色是否已经三子连线。
    bool hasWon(int role) const;

    // 判断棋盘是否已经没有空位。
    bool isFull() const;

    // 按截图风格打印棋盘。
    void print() const;

    // 获取所有空位，供电脑选择落子。
    std::vector<std::pair<int, int> > emptyCells() const;

private:
    int size_;
    std::vector<std::vector<int> > board_;
};

#endif
