# CMakeList.txt: SIQRS 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("SIQRS")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
find_package(GMP REQUIRED)
INCLUDE_DIRECTORIES(${GMP_INCLUDE_DIR})


# 将源代码添加到此项目的可执行文件。
add_executable (SIQRS "SIQRS.cpp" "SIQRS.h" "FiniteField.h" "Curve.h" "GmpTest.cc" "GmpTest.h")
target_link_libraries(SIQRS ${GMP_LIBRARIES})


# TODO: 如有需要，请添加测试并安装目标。
