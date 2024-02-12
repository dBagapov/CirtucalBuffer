cmake_minimum_required(VERSION 3.25)
project(labwork_8_Reddle04)

set(CMAKE_CXX_STANDARD 23)

add_executable(labwork_8_Reddle04 main.cpp lib/CCircularBufferExp.h lib/CCircularBuffer.h)

enable_testing()
add_subdirectory(tests)
