cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_Delay STATIC
    src/AP_Delay.cpp
)

# Assuming the path to the AP_AudioIO include directory is relative to where this CMakeLists.txt is located.
# You might need to adjust the relative path if the structure is different.
target_include_directories(AP_Delay PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# You need to ensure this path is correct relative to the location of this CMakeLists.txt
target_include_directories(AP_Delay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../AP_AudioIO/include)

target_include_directories(AP_Delay PRIVATE ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include)

# Linking against AP_Audio_Effect library
target_link_libraries(AP_Delay PRIVATE AP_Audio_Effect)

