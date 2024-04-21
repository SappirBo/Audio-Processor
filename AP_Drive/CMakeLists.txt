cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_Drive STATIC
    src/AP_Drive.cpp
    include/AP_Drive.hpp  # Include this if you have the declaration here
)

# Including directories for the interface of this library (used when other targets link to this library)
target_include_directories(AP_Drive PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Include the AP_AudioIO module. Assuming the path is relative to this CMakeLists.txt.
target_include_directories(AP_Drive PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../AP_AudioIO/include)

# Include the AP_Audio_Effect module
# Assuming the path is relative to the root CMakeLists.txt.
target_include_directories(AP_Drive PRIVATE ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include)

# Linking against AP_Audio_Effect library
target_link_libraries(AP_Drive PRIVATE AP_Audio_Effect)
