cmake_minimum_required(VERSION 3.17)
project(system-labs)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1.out lab1.cpp)

enable_testing()
add_test(NAME lab1 COMMAND lab1.out)
