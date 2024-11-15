cmake_minimum_required(VERSION 3.5.0)
project(demo VERSION 0.1.0 LANGUAGES CXX)

add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE python3.8)
target_include_directories(demo PRIVATE /usr/include/python3.8)
