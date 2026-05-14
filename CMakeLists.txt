cmake_minimum_required(VERSION 4.0.0)

project(jit)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(jit ${SOURCES})