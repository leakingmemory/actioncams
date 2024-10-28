cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)
add_library(ff OBJECT FFCtx.cpp FFCtx.h)

target_include_directories(ff PRIVATE ${LIBAV_INCLUDE_DIRS})
