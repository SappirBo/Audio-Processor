cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AudioIO STATIC
    src/AudioIO.cpp
)

# Specify include directories for the AudioIO library
target_include_directories(AudioIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
