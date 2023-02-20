cmake_minimum_required(VERSION 3.24)
project(curriculum_cpp)
# 改变编码
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")
#set(CMAKE_CXX_STANDARD 17)
link_libraries(libeasyx.a)
add_executable(curriculum_cpp main.cpp include/sys.h realization/sys.cpp include/bank.h realization/bank.cpp include/times.h realization/times.cpp)
