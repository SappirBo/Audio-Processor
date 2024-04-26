cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_Compressor STATIC
    src/AP_Compressor.cpp
)

target_include_directories(AP_Compressor PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(AP_Compressor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../AP_AudioIO/include)

target_include_directories(AP_Compressor PRIVATE ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include)

target_link_libraries(AP_Compressor PRIVATE AP_Audio_Effect)

