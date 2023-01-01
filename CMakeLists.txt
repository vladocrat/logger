cmake_minimum_required(VERSION 3.5)

project(logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(logger
    logger.h
    logger.cpp
    filelogger.h
    Constants.h main.cpp)

install(TARGETS logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
