#ifndef SOLVER_TYPES_H
#define SOLVER_TYPES_H

#include "Queens.h"
#include <string>

// 求解器的返回结果
struct SolverResult {
    CountType solution_count; // 合法解的总数
    double elapsed_seconds;   // 求解耗时（秒）
};

// 将128位整数转换为十进制字符串（标准库不支持直接输出 __int128）
std::string count_to_string(CountType value);

#endif
