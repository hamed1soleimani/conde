cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 17)

include_directories(..)

file(GLOB TEST_LIST
    "*.cpp"
)

foreach(TEST_FILE ${TEST_LIST})
	get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
	add_executable(${TEST_NAME} ${TEST_FILE}) 
	target_link_libraries(${TEST_NAME} conde)
	target_link_libraries(${TEST_NAME} gtest gtest_main pthread)
endforeach()
