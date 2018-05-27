cmake_minimum_required(VERSION 3.10)
project(conde)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES_LIST
        hash_table.cpp)

set(HEADERS_LIST
        hash_table.h
        hash_table_inl.h
        tuple_generator.h)

add_library(conde SHARED
        ${SOURCES_LIST}
        ${HEADERS_LIST})

target_link_libraries(conde pthread tbb tbbmalloc)

add_subdirectory(test)
