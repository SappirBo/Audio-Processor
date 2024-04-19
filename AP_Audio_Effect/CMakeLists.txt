# Set the minimum required version of cmake for this project
cmake_minimum_required(VERSION 3.10)

# Define the project and specify the relevant details
project(AP_Audio_Effect VERSION 1.0 DESCRIPTION "Base Audio Effects Library")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library target
add_library(AP_Audio_Effect STATIC
    src/AP_Audio_Effect.cpp  # Source file for the base class
    include/AP_Audio_Effect.hpp  # Header file for the base class
)

# Specify include directories for this library
target_include_directories(AP_Audio_Effect PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
