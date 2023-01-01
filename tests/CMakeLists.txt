cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

set(GOOGLE_TEST_DIR ${CMAKE_SOURCE_DIR}/googletest/include)
set(LOGGER_DIR ${CMAKE_SOURCE_DIR}/src)

enable_testing()
include_directories(${GOOGLE_TEST_DIR})
include_directories(${LOGGER_DIR})

message(${LOGGER_DIR})

add_executable(tests main.cpp
                inputtests.h
                globaltests.h
                lib.h
                ${LOGGER_DIR}/logger.cpp)

#target_include_directories(tests PUBLIC ${LOGGER_DIR})
target_link_libraries(tests gtest_main gtest)
