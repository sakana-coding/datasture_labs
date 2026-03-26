#include "Random.h"

#include <stdexcept>

// 默认随机种子：每次运行结果一般不同。
Random::Random() : generator_(std::random_device{}()) {}

// 固定随机种子：便于复现同一组结果。
Random::Random(unsigned int seed) : generator_(seed) {}

// 按泊松分布生成随机整数。
// mean 越大，平均每个时间单位生成的请求数越多。
int Random::poisson(double mean) {
    if (mean < 0.0) {
        throw std::invalid_argument("Poisson mean must be nonnegative.");
    }
    std::poisson_distribution<int> distribution(mean);
    return distribution(generator_);
}
