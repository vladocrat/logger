cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

set(LOGGER_DIR ${CMAKE_SOURCE_DIR}/logger/src)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include_directories(${LOGGER_DIR})

add_executable(tests main.cpp
                inputtests.h
                globaltests.h)

target_link_libraries(tests gtest_main logger)
