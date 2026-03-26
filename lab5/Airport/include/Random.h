#ifndef AIRPORT_RANDOM_H
#define AIRPORT_RANDOM_H

#include <random>

// Random 类用于生成随机请求数。
// 本实验里只使用 poisson(mean) 来模拟“平均每单位时间的到达/离港架次”。
class Random {
public:
    // 使用随机设备生成种子。
    Random();
    // 使用固定种子，便于测试。
    explicit Random(unsigned int seed);

    // 返回服从泊松分布的随机整数。
    int poisson(double mean);

private:
    // 随机数引擎。
    std::mt19937 generator_;
};

#endif
