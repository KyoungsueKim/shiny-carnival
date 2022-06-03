cmake_minimum_required(VERSION 3.10)
 
# configure a header file to pass some of the CMake settings
# to the source code

project(shiny-carnival)
# set(CMAKE_VERBOSE_MAKEFILE true)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
 
add_executable(main ${CMAKE_SOURCE_DIR}/shiny-carnival/src/main.cpp ${CMAKE_SOURCE_DIR}/shiny-carnival/src/piezo.cpp ${CMAKE_SOURCE_DIR}/shiny-carnival/src/sensor.cpp)
# add the executable

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/src)

