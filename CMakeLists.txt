cmake_minimum_required(VERSION 3.10.2)

project(kvstore2pcsystem)

aux_source_directory(. DIR_SRCS)

set(CMAKE_CXX_FLAGS "-D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0 ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS " -fPIC -rdynamic -pthread ${CMAKE_EXE_LINKER_FLAGS}")

INCLUDE_DIRECTORIES( ./src/include ./src/third_party/parallel_hashmap ./src/third_party/ossSocket)

add_executable( kvstore2pcsystem
                ./src/main.cc
                ./src/parser.cc
                ./src/kv.cc

                ./src/test/parser_test.cc
                ./src/test/kv_test.cc
                )