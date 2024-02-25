cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME wfc)

project(${PROJECT_NAME} LANGUAGES CXX C)

add_subdirectory(3rdlibs)
add_subdirectory(acid)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR "Ninja")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

option(ACID_PLATFORM_WINDOWS "Windows" OFF)
option(ACID_PLATFORM_APPLE "Apple" OFF)
option(ACID_DEBUG "Debug" ON)

if (WIN32)
    message(STATUS "Using Platform Windows")
    set(ACID_PLATFORM_WINDOWS ON)
elseif (APPLE)
    message(STATUS "Using Platform Apple")
    set(ACID_PLATFORM_APPLE ON)
endif()


