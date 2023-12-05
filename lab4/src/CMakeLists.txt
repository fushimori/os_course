cmake_minimum_required(VERSION 3.10)
project(lab_4)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(GCF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GCF)
set(E_DIR ${CMAKE_CURRENT_SOURCE_DIR}/E)

add_library(GCF SHARED
    ${GCF_DIR}/gcf.cpp
    ${GCF_DIR}/gcf.h
)
add_library(EXP SHARED
    ${E_DIR}/exp.cpp
    ${E_DIR}/exp.h
)

add_executable(lab4
    main.cpp
)

#add_executable(example
#    main1.cpp
#)

target_link_libraries(lab4 PRIVATE EXP GCF)
#target_link_libraries(example PRIVATE EXP GCF)

enable_testing()
add_executable(test_lab4 
    test.cpp
    ${GCF_DIR}/gcf.cpp
    ${GCF_DIR}/gcf.h
    ${E_DIR}/exp.cpp
    ${E_DIR}/exp.h)

target_link_libraries(test_lab4 ${GTEST_LIBRARIES})
add_test(NAME counter_test COMMAND test_lab4)
