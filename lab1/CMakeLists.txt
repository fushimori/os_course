 cmake_minimum_required(VERSION 3.10)
 project(lab1)

 add_executable(main src/main.cpp )
 add_executable(child src/child.cpp src/composite.cpp src/composite.h)
 add_executable(flread src/filereader.cpp)