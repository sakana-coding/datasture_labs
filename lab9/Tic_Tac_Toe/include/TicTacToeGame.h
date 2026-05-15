#ifndef TIC_TAC_TOE_GAME_H
#define TIC_TAC_TOE_GAME_H

#include "ComputerPlayer.h"
#include "TicTacToeBoard.h"

#include <string>

/**
 * 游戏控制类：
 * 负责完整流程、输入校验、回合切换和最终结果输出。
 */
class TicTacToeGame {
public:
    TicTacToeGame();
    void run();

private:
    enum GameMode {
        HUMAN_VS_COMPUTER = 1,
        COMPUTER_VS_COMPUTER = 2
    };

    TicTacToeBoard board_;
    ComputerPlayer computer1_;
    ComputerPlayer computer2_;
    GameMode mode_;
    bool humanFirst_;

    // 游戏开始前读取棋盘大小和电脑难度。
    bool setupGame();

    // 电脑回合：落子后输出 Computer1: 和棋盘。
    void computerTurn(const std::string& name,
                      ComputerPlayer& computer,
                      int myRole,
                      int opponentRole);

    // 电脑对电脑模式下，每次电脑落子前暂停，等待用户输入任意内容后继续。
    bool waitBeforeComputerMove(const std::string& name) const;

    // 人机对战主循环。
    void runHumanVsComputer();

    // 电脑对电脑主循环。
    void runComputerVsComputer();

    // 玩家回合：读取合法且未被占用的位置，然后落子并打印棋盘。
    bool playerTurn();

    // 读取一个 0~2 的整数坐标。
    bool readCoordinate(const std::string& name, int& value) const;

    // 读取任意指定范围内的整数，用于棋盘大小和难度选择。
    bool readIntegerInRange(const std::string& label,
                            int minValue,
                            int maxValue,
                            int& value) const;

    // 严格解析整数：hello、2abc、空行、越界数字都会被判为非法。
    bool parseIntegerInRange(const std::string& line,
                             int minValue,
                             int maxValue,
                             int& value) const;

    // 非交互测试时回显输入，让输出看起来仍像手动键入。
    void echoInputWhenRedirected(const std::string& line) const;

    // 判断用户是否输入 quit/q，支持中途退出。
    bool isQuitCommand(const std::string& line) const;

    // 检查是否结束，并输出最终结果。
    bool isGameOver() const;
};

#endif
