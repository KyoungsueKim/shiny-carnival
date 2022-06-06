cmake_minimum_required(VERSION 3.10)
 
# configure a header file to pass some of the CMake settings
# to the source code

project(shiny-carnival)
# set(CMAKE_VERBOSE_MAKEFILE true)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(SRC_DIRECTORY ${CMAKE_SOURCE_DIR}/shiny-carnival/src)
 
add_executable(main ${SRC_DIRECTORY}/main.cpp ${SRC_DIRECTORY}/piezo.cpp ${SRC_DIRECTORY}/sensor.cpp ${SRC_DIRECTORY}/pwm.cpp ${SRC_DIRECTORY}/gpio.cpp)

# add the executable
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/src)

