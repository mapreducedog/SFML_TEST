cmake_minimum_required(VERSION 2.6)
project(sfml_test)
set(CMAKE_CXX_FLAGS "-std=gnu++11")
add_executable(sfml_test main.cpp)
target_link_libraries(main.o -lsfml-graphics)
link_libraries(
add_library(g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
install(TARGETS sfml_test RUNTIME DESTINATION bin)
