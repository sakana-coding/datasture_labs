#include "ComputerPlayer.h"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <vector>

ComputerPlayer::ComputerPlayer(Difficulty difficulty)
    : difficulty_(difficulty) {
}

void ComputerPlayer::setDifficulty(Difficulty difficulty) {
    difficulty_ = difficulty;
}

ComputerPlayer::Difficulty ComputerPlayer::difficulty() const {
    return difficulty_;
}

std::pair<int, int> ComputerPlayer::chooseMove(TicTacToeBoard& board) const {
    return chooseMove(board, TicTacToeBoard::COMPUTER, TicTacToeBoard::PLAYER);
}

std::pair<int, int> ComputerPlayer::chooseMove(TicTacToeBoard& board,
                                               int myRole,
                                               int opponentRole) const {
    if (difficulty_ == EASY) {
        return chooseEasyMove(board);
    }
    if (difficulty_ == HARD) {
        return chooseHardMove(board, myRole, opponentRole);
    }
    return chooseNormalMove(board, myRole, opponentRole);
}

std::pair<int, int> ComputerPlayer::chooseEasyMove(const TicTacToeBoard& board) const {
    std::vector<std::pair<int, int> > cells = board.emptyCells();
    if (cells.empty()) {
        return std::make_pair(-1, -1);
    }

    // 简单模式故意“不思考”，直接选择扫描到的第一个空位。
    return cells.front();
}

std::pair<int, int> ComputerPlayer::chooseNormalMove(TicTacToeBoard& board,
                                                     int myRole,
                                                     int opponentRole) const {
    std::pair<int, int> move;

    // 优先尝试电脑自己能否一步获胜。
    if (findWinningMove(board, myRole, move)) {
        return move;
    }

    // 如果玩家下一步能赢，电脑必须阻挡。
    if (findWinningMove(board, opponentRole, move)) {
        return move;
    }

    // 3x3 时保留原来的固定优先级，第一步仍然落在右下角，贴近原截图。
    if (board.size() == TicTacToeBoard::DEFAULT_SIZE) {
        static const int priority[][2] = {
            {2, 2}, {2, 1}, {2, 0},
            {1, 2}, {1, 1}, {1, 0},
            {0, 2}, {0, 1}, {0, 0}
        };

        for (int k = 0; k < 9; ++k) {
            int x = priority[k][0];
            int y = priority[k][1];
            if (board.isEmpty(x, y)) {
                return std::make_pair(x, y);
            }
        }
    }

    // 大棋盘普通模式：优先中心附近，减少明显很差的边角起手。
    int n = board.size();
    std::pair<int, int> bestMove(-1, -1);
    int bestDistance = std::numeric_limits<int>::max();
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            if (!board.isEmpty(x, y)) {
                continue;
            }

            int dx = 2 * x - (n - 1);
            int dy = 2 * y - (n - 1);
            int distance = dx * dx + dy * dy;
            if (distance < bestDistance) {
                bestDistance = distance;
                bestMove = std::make_pair(x, y);
            }
        }
    }

    return bestMove;
}

std::pair<int, int> ComputerPlayer::chooseHardMove(TicTacToeBoard& board,
                                                   int myRole,
                                                   int opponentRole) const {
    std::pair<int, int> move;

    // 任何棋盘都先处理一步胜利和一步防守，这是最高优先级。
    if (findWinningMove(board, myRole, move)) {
        return move;
    }
    if (findWinningMove(board, opponentRole, move)) {
        return move;
    }

    // 3x3 棋盘可以完整搜索全部可能局面，电脑不会主动走输。
    if (board.size() == TicTacToeBoard::DEFAULT_SIZE) {
        int bestScore = std::numeric_limits<int>::min();
        std::pair<int, int> bestMove(-1, -1);
        std::vector<std::pair<int, int> > cells = board.emptyCells();

        for (std::size_t i = 0; i < cells.size(); ++i) {
            int x = cells[i].first;
            int y = cells[i].second;

            board.place(x, y, myRole);
            int score = minimax(board, false, 0,
                                std::numeric_limits<int>::min(),
                                std::numeric_limits<int>::max(),
                                myRole,
                                opponentRole);
            board.undo(x, y);

            if (score > bestScore) {
                bestScore = score;
                bestMove = std::make_pair(x, y);
            }
        }

        return bestMove;
    }

    // 大棋盘局面数量太多，不能完整搜索，改用启发式评分保证速度。
    return chooseHeuristicMove(board, myRole, opponentRole);
}

bool ComputerPlayer::findWinningMove(TicTacToeBoard& board,
                                     int role,
                                     std::pair<int, int>& move) const {
    std::vector<std::pair<int, int> > cells = board.emptyCells();
    for (std::size_t i = 0; i < cells.size(); ++i) {
        int x = cells[i].first;
        int y = cells[i].second;

        board.place(x, y, role);
        bool win = board.hasWon(role);
        board.undo(x, y);

        if (win) {
            move = std::make_pair(x, y);
            return true;
        }
    }
    return false;
}

int ComputerPlayer::minimax(TicTacToeBoard& board,
                            bool computerTurn,
                            int depth,
                            int alpha,
                            int beta,
                            int myRole,
                            int opponentRole) const {
    if (board.hasWon(myRole)) {
        return 10 - depth;
    }
    if (board.hasWon(opponentRole)) {
        return depth - 10;
    }
    if (board.isFull()) {
        return 0;
    }

    std::vector<std::pair<int, int> > cells = board.emptyCells();

    if (computerTurn) {
        int bestScore = std::numeric_limits<int>::min();
        for (std::size_t i = 0; i < cells.size(); ++i) {
            int x = cells[i].first;
            int y = cells[i].second;

            board.place(x, y, myRole);
            int score = minimax(board, false, depth + 1, alpha, beta,
                                myRole, opponentRole);
            board.undo(x, y);

            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            if (beta <= alpha) {
                break;
            }
        }
        return bestScore;
    }

    int bestScore = std::numeric_limits<int>::max();
    for (std::size_t i = 0; i < cells.size(); ++i) {
        int x = cells[i].first;
        int y = cells[i].second;

        board.place(x, y, opponentRole);
        int score = minimax(board, true, depth + 1, alpha, beta,
                            myRole, opponentRole);
        board.undo(x, y);

        bestScore = std::min(bestScore, score);
        beta = std::min(beta, bestScore);
        if (beta <= alpha) {
            break;
        }
    }
    return bestScore;
}

std::pair<int, int> ComputerPlayer::chooseHeuristicMove(TicTacToeBoard& board,
                                                        int myRole,
                                                        int opponentRole) const {
    std::vector<std::pair<int, int> > cells = board.emptyCells();
    std::pair<int, int> bestMove(-1, -1);
    int bestScore = std::numeric_limits<int>::min();

    for (std::size_t i = 0; i < cells.size(); ++i) {
        int x = cells[i].first;
        int y = cells[i].second;

        board.place(x, y, myRole);
        int score = evaluateBoard(board, myRole, opponentRole)
                  - evaluateBoard(board, opponentRole, myRole);
        board.undo(x, y);

        // 分数相同时，优先选择更靠近中心的位置。
        int n = board.size();
        int dx = 2 * x - (n - 1);
        int dy = 2 * y - (n - 1);
        int centerBonus = 1000 - (dx * dx + dy * dy);
        score = score * 1000 + centerBonus;

        if (score > bestScore) {
            bestScore = score;
            bestMove = std::make_pair(x, y);
        }
    }

    return bestMove;
}

int ComputerPlayer::evaluateBoard(TicTacToeBoard& board,
                                  int role,
                                  int opponent) const {
    int n = board.size();
    int score = 0;

    // 由于棋盘类没有暴露内部数组，这里通过“临时落子/撤销”不方便读取。
    // 评分只需要知道每条线是否仍有潜力，所以改用试探方式统计：
    // 对每个空位尝试落 role，能形成更接近胜利的局面就加分。
    std::vector<std::pair<int, int> > cells = board.emptyCells();
    for (std::size_t i = 0; i < cells.size(); ++i) {
        int x = cells[i].first;
        int y = cells[i].second;

        board.place(x, y, role);
        if (board.hasWon(role)) {
            score += 100000;
        } else {
            // 中心附近加少量分，作为大棋盘启发式。
            int dx = 2 * x - (n - 1);
            int dy = 2 * y - (n - 1);
            score += 100 - (dx * dx + dy * dy);
        }
        board.undo(x, y);

        board.place(x, y, opponent);
        if (board.hasWon(opponent)) {
            score -= 50000;
        }
        board.undo(x, y);
    }

    return score;
}

int ComputerPlayer::evaluateLine(int roleCount, int opponentCount) const {
    if (roleCount > 0 && opponentCount > 0) {
        return 0;
    }
    if (roleCount == 0) {
        return 1;
    }
    int score = 1;
    for (int i = 0; i < roleCount; ++i) {
        score *= 10;
    }
    return score;
}
