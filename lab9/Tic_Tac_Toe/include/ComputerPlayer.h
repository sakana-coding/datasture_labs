#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "TicTacToeBoard.h"

#include <utility>

/**
 * 电脑玩家类：
 * 1. 如果自己下一步能赢，直接赢；
 * 2. 如果玩家下一步能赢，先堵住玩家；
 * 3. 否则按固定优先级选择空位。
 */
class ComputerPlayer {
public:
    enum Difficulty {
        EASY = 1,    // 简单：不看胜负，按顺序落在第一个空位。
        NORMAL = 2,  // 普通：能赢就赢，玩家要赢就堵，否则按固定优先级落子。
        HARD = 3     // 困难：3x3 使用极小化极大算法；更大棋盘使用启发式评分。
    };

    explicit ComputerPlayer(Difficulty difficulty = NORMAL);

    void setDifficulty(Difficulty difficulty);
    Difficulty difficulty() const;

    // 默认用于“人机对战”：电脑执 1，玩家执 2。
    std::pair<int, int> chooseMove(TicTacToeBoard& board) const;

    // 可指定电脑自己的棋子和对手棋子，用于“电脑 vs 电脑”。
    std::pair<int, int> chooseMove(TicTacToeBoard& board,
                                   int myRole,
                                   int opponentRole) const;

private:
    Difficulty difficulty_;

    // 试探 role 是否存在“一步成三”的落子位置。
    bool findWinningMove(TicTacToeBoard& board,
                         int role,
                         std::pair<int, int>& move) const;

    std::pair<int, int> chooseEasyMove(const TicTacToeBoard& board) const;
    std::pair<int, int> chooseNormalMove(TicTacToeBoard& board,
                                         int myRole,
                                         int opponentRole) const;
    std::pair<int, int> chooseHardMove(TicTacToeBoard& board,
                                       int myRole,
                                       int opponentRole) const;

    // 困难模式 3x3 专用：极小化极大搜索，保证最优下法。
    int minimax(TicTacToeBoard& board,
                bool computerTurn,
                int depth,
                int alpha,
                int beta,
                int myRole,
                int opponentRole) const;

    // 大棋盘困难模式用的简单启发式评分。
    std::pair<int, int> chooseHeuristicMove(TicTacToeBoard& board,
                                            int myRole,
                                            int opponentRole) const;
    int evaluateBoard(TicTacToeBoard& board,
                      int role,
                      int opponentRole) const;
    int evaluateLine(int roleCount, int opponentCount) const;
};

#endif
