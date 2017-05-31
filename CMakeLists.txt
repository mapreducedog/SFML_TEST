cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=gnu++11 -pthread -lX11")
#set(CMAKE_CXX_FLAGS "-std=gnu++11 -m64 -march=native -pthread -lX11")
project(myproject)

set(SFML_STATIC_LIBS FALSE CACHE BOOL "Choose whether SFML is linked statically or shared.")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/src")

set(EXECUTABLE_NAME "myproject")
add_executable(${EXECUTABLE_NAME} main.cpp)

#Add libraries here
add_subdirectory(src)
target_link_libraries(${EXECUTABLE_NAME} bubbles_lib)






set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
include_directories(/usr/include)
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
 message(${SFML_INCLUDE_DIR})
 message(${SFML_LIBRARIES})
 include_directories(${SFML_INCLUDE_DIR})
 target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()
#target_link_libraries(${EXECUTABLE_NAME} /usr/lib/x86_64-linux-gnu/libsfml-system.so /usr/lib/x86_64-linux-gnu/libsfml-window.so /usr/lib/x86_64-linux-gnu/libsfml-graphics.so /usr/lib/x86_64-linux-gnu/libsfml-network.so /usr/lib/x86_64-linux-gnu/libsfml-audio.so)

if (SFML_STATIC_LIBS)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()





install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
