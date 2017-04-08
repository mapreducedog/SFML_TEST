cmake_minimum_required(VERSION 2.6)
project(sfml_test)
set(CMAKE_CXX_FLAGS "-std=gnu++11")
add_executable(sfml_test main.cpp)

install(TARGETS sfml_test RUNTIME DESTINATION bin)
