#include "Utility.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

// 按行读取一个整数，并检查是否在闭区间 [min_value, max_value] 内。
int read_int_in_range(const std::string& prompt,
                      const std::string& error_message,
                      int min_value,
                      int max_value) {
    std::string line;
    while (true) {
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin >> std::ws, line)) {
            std::cin.clear();
            continue;
        }

        std::istringstream input(line);
        int value = 0;
        char extra = '\0';
        if ((input >> value) && !(input >> extra) &&
            value >= min_value && value <= max_value) {
            return value;
        }
        std::cout << error_message << std::endl;
    }
}

// 按行读取一个实数，并检查是否在闭区间 [min_value, max_value] 内。
double read_double_in_range(const std::string& prompt,
                            const std::string& error_message,
                            double min_value,
                            double max_value) {
    std::string line;
    while (true) {
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin >> std::ws, line)) {
            std::cin.clear();
            continue;
        }

        std::istringstream input(line);
        double value = 0.0;
        char extra = '\0';
        if ((input >> value) && !(input >> extra) &&
            value >= min_value && value <= max_value) {
            return value;
        }
        std::cout << error_message << std::endl;
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

    queue_limit = read_int_in_range(
        "Up to what number of planes can be waiting to land or take off at any time? ",
        "queue_limit must be integer, and queue_limit>=1 and queue_limit<=100",
        1,
        100);

    end_time = read_int_in_range(
        "How many units of time will the simulation run?",
        "end_time must be integer, and end_time>=1 and end_time<=20",
        1,
        20);

    arrival_rate = read_double_in_range(
        "Expected number of arrivals per unit time?",
        "arrival_rate must be double, and arrival_rate>=0 and arrival_rate<=1",
        0.0,
        1.0);

    departure_rate = read_double_in_range(
        "Expected number of departures per unit time?",
        "departure_rate must be double, and departure_rate>=0 and departure_rate<=1",
        0.0,
        1.0);

    // 当总流量大于 1 时，意味着平均每个时间单位到来的需求超过单跑道处理能力，
    // 程序给出饱和警告，但仍允许继续模拟。
    if (arrival_rate + departure_rate > 1.0) {
        std::cout << "Safety Warning: This airport will become saturated." << std::endl;
    }
}

// 输出跑道空闲信息。
void run_idle(int time) {
    std::cout << time << ": Runway is idle." << std::endl;
}
