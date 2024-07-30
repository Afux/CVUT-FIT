cmake_minimum_required(VERSION 3.27)
project(PT3)

set(CMAKE_CXX_STANDARD 20)

add_executable(PT3 sample.cpp)
target_link_libraries(${PROJECT_NAME} crypto)
