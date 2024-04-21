cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_AudioIO STATIC
    src/AP_AudioIO.cpp
)

# Specify include directories for the AP_AudioIO library
target_include_directories(AP_AudioIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
