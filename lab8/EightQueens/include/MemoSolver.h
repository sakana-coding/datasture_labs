#ifndef MEMO_SOLVER_H
#define MEMO_SOLVER_H

#include "SolverTypes.h"

// 使用位标记优化的回溯法求解 N 皇后，返回解的数量和耗时
SolverResult solve_memoized(int board_size);

#endif
