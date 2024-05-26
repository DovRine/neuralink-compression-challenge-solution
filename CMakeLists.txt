cmake_minimum_required(VERSION 3.10)

project(CompressionProject)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

find_package(SndFile REQUIRED)

add_executable(encode src/main_encode.cpp src/encode.cpp)
target_link_libraries(encode SndFile::sndfile)

add_executable(decode src/main_decode.cpp src/decode.cpp)
target_link_libraries(decode SndFile::sndfile)
