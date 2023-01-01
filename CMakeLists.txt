cmake_minimum_required(VERSION 3.5)

project(logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_subdirectory(googletest)
add_subdirectory(tests)

include_directories(src)

add_executable(logger
    src/logger.h src/logger.cpp
    src/filelogger.h
    src/Constants.h
    src/main.cpp)

install(TARGETS logger tests
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
