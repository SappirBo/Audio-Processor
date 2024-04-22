cmake_minimum_required(VERSION 3.10)

# Project name and a few useful settings. Other commands can pick up the results
project(AudioProject VERSION 1.0 DESCRIPTION "An Audio Processing Application")

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler options
add_compile_options(-Wall -Wextra -pedantic)

# Include subdirectory
add_subdirectory(AP_AudioIO)
add_subdirectory(AP_Drive)
add_subdirectory(AP_Delay)
add_subdirectory(AP_Audio_Effect)
add_subdirectory(AP_WavPlayer)

# Add executable for the main application
add_executable(AudioApp main.cpp)

# Link the AP_AudioIO library and the AP_Drive module to the executable
target_link_libraries(AudioApp PRIVATE 
    AP_AudioIO
    AP_Audio_Effect
    AP_Drive 
    AP_Delay
    AP_WavPlayer
)


target_include_directories(AudioApp PRIVATE 
    ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include
    ${PROJECT_SOURCE_DIR}/AP_Delay/include
    ${PROJECT_SOURCE_DIR}/AP_Drive/include
    ${PROJECT_SOURCE_DIR}/AP_AudioIO/include
    ${PROJECT_SOURCE_DIR}/AP_WavPlayer/include
)

