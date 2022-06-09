cmake_minimum_required(VERSION 3.10)
 
# configure a header file to pass some of the CMake settings
# to the source code

project(shiny-carnival)
# set(CMAKE_VERBOSE_MAKEFILE true)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(SRC_DIRECTORY ${CMAKE_SOURCE_DIR}/shiny-carnival/src)

add_executable(main ${SRC_DIRECTORY}/main.cpp ${SRC_DIRECTORY}/piezo.cpp ${SRC_DIRECTORY}/sensor.cpp ${SRC_DIRECTORY}/pwm.c ${SRC_DIRECTORY}/gpio.c ${SRC_DIRECTORY}/socket.cpp ${SRC_DIRECTORY}/camera.cpp ${SRC_DIRECTORY}/client.cpp ${SRC_DIRECTORY}/client.h ${SRC_DIRECTORY}/server.cpp ${SRC_DIRECTORY}/server.h)

SET(OpenCV_DIR /usr/local/include/opencv2/)
find_package(OpenCV REQUIRED)

include_directories(/usr/local/include/opencv2/)
target_link_libraries(main pthread ${OpenCV_LIBS})