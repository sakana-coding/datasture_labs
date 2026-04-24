#include "PlainSolver.h"

#include "Queens.h"

#include <ctime>
#include <stdexcept>

// 朴素回溯：逐行尝试每一列，递归枚举所有合法摆放
CountType plain_backtrack(Queens& board) {
    if (board.is_solved()) {
        return 1;
    }

    CountType sum = 0;
    for (int col = 0; col < board.board_size; ++col) {
        if (board.unguarded(col)) {
            board.insert(col);
            sum += plain_backtrack(board);
            board.remove(col);
        }
    }
    return sum;
}

SolverResult solve_plain(int board_size) {
    if (board_size <= 0 || board_size > max_board) {
        throw std::invalid_argument("board size is out of range");
    }

    SolverResult result;
    result.solution_count = 0;
    result.elapsed_seconds = 0;

    Queens board(board_size);

    clock_t start_time = clock();
    result.solution_count = plain_backtrack(board);
    clock_t end_time = clock();

    result.elapsed_seconds =
        static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;

    return result;
}
