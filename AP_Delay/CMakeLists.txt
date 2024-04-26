cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(AP_Delay STATIC
    src/AP_Delay.cpp
)

target_include_directories(AP_Delay PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(AP_Delay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../AP_AudioIO/include)

target_include_directories(AP_Delay PRIVATE ${PROJECT_SOURCE_DIR}/AP_Audio_Effect/include)

target_link_libraries(AP_Delay PRIVATE AP_Audio_Effect)

