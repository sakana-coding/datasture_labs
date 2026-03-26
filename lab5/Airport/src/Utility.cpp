#include "Utility.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

// 通用读入函数：
// 1. 按行读取，避免 cin 失败后卡死
// 2. 检查类型是否合法
// 3. 检查是否满足调用者给出的范围条件
template <typename T, typename Validator>
T read_value(const std::string& prompt, const std::string& error_message, Validator validator) {
    std::string line;
    while (true) {
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin >> std::ws, line)) {
            std::cin.clear();
            continue;
        }

        std::istringstream input(line);
        T value{};
        char extra = '\0';
        if ((input >> value) && !(input >> extra) && validator(value)) {
            return value;
        }
        std::cerr << error_message << std::endl;
    }
}

}  // namespace

// 从终端读取仿真参数。
// queue_limit   : 每个等待队列允许的最大长度
// end_time      : 仿真持续的总时间单位数
// arrival_rate  : 平均每单位时间到达的飞机数
// departure_rate: 平均每单位时间离港的飞机数
void initialize(int& end_time, int& queue_limit, double& arrival_rate, double& departure_rate) {
    std::cout << "This program simulates an airport with only one runway." << std::endl
              << "One plane can land or depart in each unit of time." << std::endl;

    queue_limit = read_value<int>(
        "Up to what number of planes can be waiting to land or take off at any time? ",
        "Queue limit must be an integer greater than 0.",
        [](int value) { return value > 0; });

    end_time = read_value<int>(
        "How many units of time will the simulation run? ",
        "Simulation time must be an integer greater than 0.",
        [](int value) { return value > 0; });

    bool acceptable = false;
    do {
        arrival_rate = read_value<double>(
            "Expected number of arrivals per unit time? ",
            "Arrival rate must be a nonnegative number.",
            [](double value) { return value >= 0.0; });

        departure_rate = read_value<double>(
            "Expected number of departures per unit time? ",
            "Departure rate must be a nonnegative number.",
            [](double value) { return value >= 0.0; });

        acceptable = true;
        // 当总流量大于 1 时，意味着平均每个时间单位到来的需求超过单跑道处理能力，
        // 程序给出饱和警告，但仍允许继续模拟。
        if (arrival_rate + departure_rate > 1.0) {
            std::cerr << "Safety Warning: This airport will become saturated." << std::endl;
        }
    } while (!acceptable);
}

// 输出跑道空闲信息。
void run_idle(int time) {
    std::cout << time << ": Runway is idle." << std::endl;
}
