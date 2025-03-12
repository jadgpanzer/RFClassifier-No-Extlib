cmake_minimum_required(VERSION 3.12)
project(RandomForestDemo VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 配置编译选项
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

# 创建可执行文件
add_executable(random_forest_demo
    src/main.cpp
    src/RandomForest.cpp
    src/DecisionTree.cpp
    src/TreeNode.cpp
    src/Dataset.cpp
)

# 包含头文件目录
target_include_directories(random_forest_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 根据构建类型设置优化级别
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g3 -O0)
    message(STATUS "Building in DEBUG mode")
else()
    add_compile_options(-O3)
    message(STATUS "Building in RELEASE mode")
endif()

# 支持多线程编译（如果编译器支持）
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-march=native -pipe)
endif()
