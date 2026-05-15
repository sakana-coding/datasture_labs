#include "TicTacToeGame.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <unistd.h>  // isatty：用于在重定向输入时主动回显测试数据。

TicTacToeGame::TicTacToeGame()
    : board_(),
      computer1_(),
      computer2_(),
      mode_(HUMAN_VS_COMPUTER),
      humanFirst_(false) {
}

void TicTacToeGame::run() {
    std::cout << "This is a Tic-Tac-Toe game.\n";

    if (!setupGame()) {
        std::cout << "Game stopped.\n";
        return;
    }

    if (mode_ == COMPUTER_VS_COMPUTER) {
        runComputerVsComputer();
    } else {
        runHumanVsComputer();
    }
}

bool TicTacToeGame::setupGame() {
    int boardSize = TicTacToeBoard::DEFAULT_SIZE;
    std::cout << "Choose board size, from "
              << TicTacToeBoard::MIN_SIZE << " to "
              << TicTacToeBoard::MAX_SIZE << ".\n";
    std::cout << "You can enter quit or q at any time to stop the game.\n";
    if (!readIntegerInRange("Board size",
                            TicTacToeBoard::MIN_SIZE,
                            TicTacToeBoard::MAX_SIZE,
                            boardSize)) {
        return false;
    }
    board_.reset(boardSize);

    int mode = HUMAN_VS_COMPUTER;
    std::cout << "Choose game mode:\n";
    std::cout << "1. Human vs Computer\n";
    std::cout << "2. Computer vs Computer\n";
    if (!readIntegerInRange("Mode", 1, 2, mode)) {
        return false;
    }
    mode_ = static_cast<GameMode>(mode);

    if (mode_ == HUMAN_VS_COMPUTER) {
        int first = 2;
        std::cout << "Choose who goes first:\n";
        std::cout << "1. Human first\n";
        std::cout << "2. Computer first\n";
        if (!readIntegerInRange("First", 1, 2, first)) {
            return false;
        }
        humanFirst_ = (first == 1);
    }

    int difficulty1 = ComputerPlayer::NORMAL;
    std::cout << "Choose Computer1 level:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Normal\n";
    std::cout << "3. Hard\n";
    if (!readIntegerInRange("Computer1 level", 1, 3, difficulty1)) {
        return false;
    }
    computer1_.setDifficulty(static_cast<ComputerPlayer::Difficulty>(difficulty1));

    if (mode_ == COMPUTER_VS_COMPUTER) {
        int difficulty2 = ComputerPlayer::NORMAL;
        std::cout << "Choose Computer2 level:\n";
        std::cout << "1. Easy\n";
        std::cout << "2. Normal\n";
        std::cout << "3. Hard\n";
        if (!readIntegerInRange("Computer2 level", 1, 3, difficulty2)) {
            return false;
        }
        computer2_.setDifficulty(static_cast<ComputerPlayer::Difficulty>(difficulty2));
    }

    return true;
}

void TicTacToeGame::runHumanVsComputer() {
    std::cout << "Wait for computer going...\n";

    // 人机模式下电脑自动落子，不需要玩家额外按回车。
    // 电脑固定是 1，玩家固定是 2；humanFirst_ 只控制谁先走。
    if (!humanFirst_) {
        computerTurn("Computer1",
                     computer1_,
                     TicTacToeBoard::COMPUTER,
                     TicTacToeBoard::PLAYER);
        if (isGameOver()) {
            return;
        }
    }

    while (true) {
        if (!playerTurn()) {
            // 输入流结束或用户输入 quit/q，直接结束程序，避免死循环。
            std::cout << "Game stopped.\n";
            return;
        }
        if (isGameOver()) {
            return;
        }

        computerTurn("Computer1",
                     computer1_,
                     TicTacToeBoard::COMPUTER,
                     TicTacToeBoard::PLAYER);
        if (isGameOver()) {
            return;
        }
    }
}

void TicTacToeGame::runComputerVsComputer() {
    std::cout << "Computer vs Computer mode. Computer1 is 1, Computer2 is 2.\n";

    while (true) {
        if (!waitBeforeComputerMove("Computer1")) {
            std::cout << "Game stopped.\n";
            return;
        }
        computerTurn("Computer1",
                     computer1_,
                     TicTacToeBoard::COMPUTER,
                     TicTacToeBoard::PLAYER);
        if (isGameOver()) {
            return;
        }

        if (!waitBeforeComputerMove("Computer2")) {
            std::cout << "Game stopped.\n";
            return;
        }
        computerTurn("Computer2",
                     computer2_,
                     TicTacToeBoard::PLAYER,
                     TicTacToeBoard::COMPUTER);
        if (isGameOver()) {
            return;
        }
    }
}

void TicTacToeGame::computerTurn(const std::string& name,
                                 ComputerPlayer& computer,
                                 int myRole,
                                 int opponentRole) {
    std::pair<int, int> move = computer.chooseMove(board_, myRole, opponentRole);
    if (move.first != -1) {
        board_.place(move.first, move.second, myRole);
    }

    std::cout << name << ":\n";
    board_.print();
}

bool TicTacToeGame::waitBeforeComputerMove(const std::string& name) const {
    std::cout << "Press Enter or input anything to let "
              << name << " go";
    std::cout << " (input quit or q to stop):";

    std::string line;
    if (!std::getline(std::cin, line)) {
        return false;
    }

    echoInputWhenRedirected(line);
    return !isQuitCommand(line);
}

bool TicTacToeGame::playerTurn() {
    while (true) {
        std::cout << "Your turn:\n";

        int x = 0;
        int y = 0;
        if (!readCoordinate("X", x)) {
            return false;
        }
        if (!readCoordinate("Y", y)) {
            return false;
        }

        if (!board_.isEmpty(x, y)) {
            std::cout << "The position is occupied, please choose another one.\n";
            continue;
        }

        board_.place(x, y, TicTacToeBoard::PLAYER);
        board_.print();
        return true;
    }
}

bool TicTacToeGame::readCoordinate(const std::string& name, int& value) const {
    std::cout << name << ":";

    std::string line;
    while (std::getline(std::cin, line)) {
        echoInputWhenRedirected(line);

        if (isQuitCommand(line)) {
            return false;
        }

        if (parseIntegerInRange(line, 0, board_.size() - 1, value)) {
            return true;
        }
        std::cout << "The " << name << " must be integer, and "
                  << name << ">=0 and " << name << "<="
                  << board_.size() - 1 << ".\n";
    }

    return false;
}

bool TicTacToeGame::readIntegerInRange(const std::string& label,
                                       int minValue,
                                       int maxValue,
                                       int& value) const {
    std::cout << label << ":";

    std::string line;
    while (std::getline(std::cin, line)) {
        echoInputWhenRedirected(line);

        if (isQuitCommand(line)) {
            return false;
        }

        if (parseIntegerInRange(line, minValue, maxValue, value)) {
            return true;
        }

        std::cout << "The " << label << " must be integer, and "
                  << label << ">=" << minValue << " and "
                  << label << "<=" << maxValue << ".\n";
    }

    return false;
}

bool TicTacToeGame::parseIntegerInRange(const std::string& line,
                                        int minValue,
                                        int maxValue,
                                        int& value) const {
    std::string text;

    // 先做一层清洗：
    // 1. 忽略空白和不可见控制字符，兼容某些终端/IDE 输入中的隐藏字符；
    // 2. 把中文输入法可能产生的全角数字 ０~９ 转成普通数字 0~9。
    for (std::size_t i = 0; i < line.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(line[i]);

        if (std::isspace(ch) || ch < 32 || ch == 127) {
            continue;
        }

        // UTF-8 全角数字：EF BC 90 ~ EF BC 99
        if (i + 2 < line.size()
            && ch == 0xEF
            && static_cast<unsigned char>(line[i + 1]) == 0xBC
            && static_cast<unsigned char>(line[i + 2]) >= 0x90
            && static_cast<unsigned char>(line[i + 2]) <= 0x99) {
            text += static_cast<char>('0'
                + static_cast<unsigned char>(line[i + 2]) - 0x90);
            i += 2;
            continue;
        }

        text += static_cast<char>(ch);
    }

    if (text.empty()) {
        return false;
    }

    int sign = 1;
    std::size_t pos = 0;
    if (text[0] == '+' || text[0] == '-') {
        sign = (text[0] == '-') ? -1 : 1;
        pos = 1;
    }
    if (pos == text.size()) {
        return false;
    }

    int number = 0;
    for (; pos < text.size(); ++pos) {
        if (!std::isdigit(static_cast<unsigned char>(text[pos]))) {
            return false;
        }

        number = number * 10 + (text[pos] - '0');
        // 提前截断，避免特别长的数字导致 int 溢出。
        if (number > maxValue + 100) {
            return false;
        }
    }

    number *= sign;
    if (number < minValue || number > maxValue) {
        return false;
    }

    value = number;
    return true;
}

void TicTacToeGame::echoInputWhenRedirected(const std::string& line) const {
    // 在终端手动输入时，系统会自动显示用户键入的内容；
    // 但用 printf/文件重定向测试时不会回显。这里主动回显，
    // 让自动测试输出也能保持类似截图的 X:1、Y:2 形式。
    if (!isatty(STDIN_FILENO)) {
        std::cout << line << '\n';
    }
}

bool TicTacToeGame::isQuitCommand(const std::string& line) const {
    std::string text;

    // 去掉首尾空白，并统一转成小写，让 " quit "、"Q" 也能退出。
    std::size_t begin = 0;
    while (begin < line.size()
           && std::isspace(static_cast<unsigned char>(line[begin]))) {
        ++begin;
    }

    std::size_t end = line.size();
    while (end > begin
           && std::isspace(static_cast<unsigned char>(line[end - 1]))) {
        --end;
    }

    for (std::size_t i = begin; i < end; ++i) {
        text += static_cast<char>(
            std::tolower(static_cast<unsigned char>(line[i])));
    }

    return text == "quit" || text == "q";
}

bool TicTacToeGame::isGameOver() const {
    if (board_.hasWon(TicTacToeBoard::COMPUTER)) {
        if (mode_ == COMPUTER_VS_COMPUTER) {
            std::cout << "Game over with computer1 win.\n";
        } else {
            std::cout << "Game over with computer win.\n";
        }
        return true;
    }
    if (board_.hasWon(TicTacToeBoard::PLAYER)) {
        if (mode_ == COMPUTER_VS_COMPUTER) {
            std::cout << "Game over with computer2 win.\n";
        } else {
            std::cout << "Game over with you win.\n";
        }
        return true;
    }
    if (board_.isFull()) {
        std::cout << "Game over with a draw.\n";
        return true;
    }
    return false;
}
