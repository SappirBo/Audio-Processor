cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_Drive STATIC
    src/AP_Drive.cpp
)

# Assuming the path to the AudioIO include directory is relative to where this CMakeLists.txt is located.
# You might need to adjust the relative path if the structure is different.
target_include_directories(AP_Drive PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# You need to ensure this path is correct relative to the location of this CMakeLists.txt
target_include_directories(AP_Drive PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../AP_AudioIO/include)
