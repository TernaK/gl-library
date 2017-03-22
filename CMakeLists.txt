cmake_minimum_required(VERSION 2.7)

project(gl_library)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# add_definitions(-DSHADERS_PATH="${gl_library_SOURCE_DIR}/src/resources/shaders/")
# add_definitions(-DTEXTURES_PATH="${gl_library_SOURCE_DIR}/src/resources/textures/")

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(src)