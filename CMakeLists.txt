cmake_minimum_required(VERSION 3.17)
project(system-labs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)

add_executable(lab1.out lab1.cpp)
add_executable(lab2.out lab2.cpp)
add_executable(lab3.out lab3.cpp lab3/gui.h lab3/gui.ui)
qt5_use_modules(lab3.out Widgets)

enable_testing()
add_test(NAME lab1 COMMAND lab1.out)
add_test(NAME lab2 COMMAND lab2.out)
