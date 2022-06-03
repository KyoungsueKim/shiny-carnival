cmake_minimum_required(VERSION 3.10)
 
# # configure a header file to pass some of the CMake settings
# # to the source code
# configure_file(TutorialConfig.h.in TutorialConfig.h)

project(shiny-carnival)
 
# add the executable
add_executable(main src/main.cpp)