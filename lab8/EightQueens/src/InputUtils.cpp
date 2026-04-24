#include "InputUtils.h"

#include "Queens.h"

#include <iostream>
#include <sstream>
#include <string>

int read_board_size() {
    std::string line;

    while (true) {
        std::cout << "What is the size of the board? ";
        // EOF 时返回 0，由调用方处理退出逻辑
        if (!std::getline(std::cin, line)) {
            return 0;
        }

        std::stringstream ss(line);
        int n;
        char extra;

        // 确保输入是单个整数且在合法范围内，extra 用于检测多余字符
        if ((ss >> n) && !(ss >> extra) && n > 0 && n <= max_board) {
            return n;
        }

        std::cout << "The number must be integer, and number>0 and number<=30" << '\n';
    }
}
