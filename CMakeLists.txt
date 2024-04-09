cmake_minimum_required(VERSION 3.10)

# Project name and a few useful settings. Other commands can pick up the results
project(AudioProject VERSION 1.0 DESCRIPTION "An Audio Processing Application")

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler options
add_compile_options(-Wall -Wextra -pedantic)

# Include the AudioIO subdirectory
add_subdirectory(AP_AudioIO)

# Add executable for the main application
add_executable(AudioApp main.cpp)

# Link the AudioIO library to the executable
target_link_libraries(AudioApp PRIVATE AudioIO)

# Include directories for the AudioIO library
target_include_directories(AudioApp PRIVATE ${PROJECT_SOURCE_DIR}/AP_AudioIO/include)
