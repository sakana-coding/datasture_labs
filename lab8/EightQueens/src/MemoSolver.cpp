#include "MemoSolver.h"

#include "Queens.h"

#include <ctime>
#include <stdexcept>
#include <vector>

// 优化回溯：用三个布尔数组分别标记列、主对角线、副对角线的占用状态
// diag1 索引 = row - col + n，diag2 索引 = row + col，保证索引非负且唯一
CountType backtrack_memo(int row,
                         int n,
                         std::vector<bool>& col_used,
                         std::vector<bool>& diag1_used,
                         std::vector<bool>& diag2_used) {
    if (row == n) {
        return 1;
    }

    CountType sum = 0;

    for (int col = 0; col < n; ++col) {
        int diag1 = row - col + n;
        int diag2 = row + col;

        if (!col_used[col] && !diag1_used[diag1] && !diag2_used[diag2]) {
            col_used[col] = true;
            diag1_used[diag1] = true;
            diag2_used[diag2] = true;

            sum += backtrack_memo(row + 1, n, col_used, diag1_used, diag2_used);

            col_used[col] = false;
            diag1_used[diag1] = false;
            diag2_used[diag2] = false;
        }
    }

    return sum;
}

SolverResult solve_memoized(int board_size) {
    if (board_size <= 0 || board_size > max_board) {
        throw std::invalid_argument("board size is out of range");
    }

    SolverResult result;
    result.solution_count = 0;
    result.elapsed_seconds = 0;

    std::vector<bool> col_used(board_size, false);
    std::vector<bool> diag1_used(2 * board_size + 1, false);
    std::vector<bool> diag2_used(2 * board_size + 1, false);

    clock_t start_time = clock();
    result.solution_count =
        backtrack_memo(0, board_size, col_used, diag1_used, diag2_used);
    clock_t end_time = clock();

    result.elapsed_seconds =
        static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;

    return result;
}
