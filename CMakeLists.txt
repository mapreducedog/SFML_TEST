cmake_minimum_required(VERSION 2.6)
project(sfml_test)

add_executable(sfml_test main.cpp)

install(TARGETS sfml_test RUNTIME DESTINATION bin)
