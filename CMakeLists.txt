cmake_minimum_required(VERSION 3.10)
project(vimbaExamples)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(examples/aquisitionCV)
add_subdirectory(examples/listCameras)