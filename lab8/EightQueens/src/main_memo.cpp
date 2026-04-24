#include "InputUtils.h"
#include "MemoSolver.h"
#include "SolverTypes.h"

#include <iomanip>
#include <iostream>

int main() {
    std::cout << "This is the Queens game2." << '\n';

    int board_size = read_board_size();
    if (board_size == 0) {
        std::cout << "Input ended." << '\n';
        return 1;
    }

    // 使用位标记优化的回溯法求解并输出结果
    SolverResult result = solve_memoized(board_size);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "It takes time: " << result.elapsed_seconds << "(s)" << '\n';
    std::cout << "The count of the answer is: "
              << count_to_string(result.solution_count) << '\n';

    return 0;
}
