cmake_minimum_required(VERSION 3.16)
project(EightQueens LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(queens_core
    src/Queens.cpp
    src/InputUtils.cpp
    src/SolverTypes.cpp
)

target_include_directories(queens_core PUBLIC include)

target_compile_options(queens_core PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra -Wpedantic>
)

add_library(queens_plain_solver src/PlainSolver.cpp)
add_library(queens_memo_solver src/MemoSolver.cpp)

target_include_directories(queens_plain_solver PUBLIC include)
target_include_directories(queens_memo_solver PUBLIC include)

target_link_libraries(queens_plain_solver PUBLIC queens_core)
target_link_libraries(queens_memo_solver PUBLIC queens_core)

target_compile_options(queens_plain_solver PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra -Wpedantic>
)

target_compile_options(queens_memo_solver PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra -Wpedantic>
)

add_executable(queens_plain src/main_plain.cpp)
add_executable(queens_memo src/main_memo.cpp)

target_link_libraries(queens_plain PRIVATE queens_plain_solver)
target_link_libraries(queens_memo PRIVATE queens_memo_solver)
