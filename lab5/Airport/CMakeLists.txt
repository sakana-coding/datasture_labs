cmake_minimum_required(VERSION 3.16)
project(AirportSimulation LANGUAGES CXX)

# 推荐在项目根目录执行：
#   cmake -S . -B build
#   cmake --build build
#   ./build/bin/airport

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 方便在终端里直接调试：若用户未显式指定构建类型，则默认使用 Debug。
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(airport
    main.cpp
    src/Plane.cpp
    src/Random.cpp
    src/Runway.cpp
    src/Utility.cpp
)

target_include_directories(airport PRIVATE ${CMAKE_SOURCE_DIR}/include)
