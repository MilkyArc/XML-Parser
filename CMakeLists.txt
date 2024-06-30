cmake_minimum_required(VERSION 3.24)
project(xml_hw)

set(CMAKE_CXX_STANDARD 17)

add_executable(xml_hw main.cpp Tree.cpp Tree.h Parser.cpp Parser.h)
