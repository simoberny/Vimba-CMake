cmake_minimum_required(VERSION 3.10)
project(listCam)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

find_package(Vimba REQUIRED)

include_directories(${Vimba_INCLUDE_DIRS})
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB allSources
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(listCam ${allSources})
target_link_libraries(listCam ${Vimba_LIBRARIES})