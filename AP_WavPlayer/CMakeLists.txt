cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_WavPlayer STATIC
    include/AP_WavPlayer.hpp
    include/portaudio.h  
)

# Including directories for the interface of this library (used when other targets link to this library)
target_include_directories(AP_WavPlayer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Include the AP_AudioIO module. Assuming the path is relative to this CMakeLists.txt.
target_include_directories(AP_WavPlayer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../AP_AudioIO/include)

# Include the AP_Audio_Effect module
# Assuming the path is relative to the root CMakeLists.txt.
target_include_directories(AP_WavPlayer PRIVATE ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include)

# Linking against AP_Audio_Effect library
target_link_libraries(AP_WavPlayer PRIVATE AP_Audio_Effect)
