cmake_minimum_required(VERSION 3.10)

# Project name and a few useful settings. Other commands can pick up the results
project(AudioProject VERSION 1.0 DESCRIPTION "An Audio Processing Application")

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler options
add_compile_options(-Wall -Wextra -pedantic)

# Include the AP_AudioIO subdirectory
add_subdirectory(AP_AudioIO)

# Include the AP_Drive subdirectory if it's a separate module
add_subdirectory(AP_Drive)

add_subdirectory(AP_Delay)

# Include the AP_Audio_Effect subdirectory
add_subdirectory(AP_Audio_Effect)


# Add executable for the main application
add_executable(AudioApp main.cpp)

# Link the AP_AudioIO library and the AP_Drive module to the executable
target_link_libraries(AudioApp PRIVATE AP_AudioIO AP_Drive AP_Delay)

# Include directories for the AP_AudioIO library
target_include_directories(AudioApp PRIVATE ${PROJECT_SOURCE_DIR}/AP_AudioIO/include)

# Include directories for the AP_Drive module if needed
target_include_directories(AudioApp PRIVATE ${PROJECT_SOURCE_DIR}/AP_Drive/include)

# Include directories for the AP_Drive module if needed
target_include_directories(AudioApp PRIVATE ${PROJECT_SOURCE_DIR}/AP_Delay/include)

# Include directories for the AP_Audio_Effect module
target_include_directories(AudioApp PRIVATE ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include)
