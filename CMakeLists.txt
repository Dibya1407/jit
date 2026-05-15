cmake_minimum_required(VERSION 4.0.0)

project(jit)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
add_executable(jit ${SOURCES})

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

target_link_libraries(jit PRIVATE OpenSSL::Crypto ZLIB::ZLIB)