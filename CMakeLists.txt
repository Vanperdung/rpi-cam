cmake_minimum_required(VERSION 3.10)
project(rpi-cam VERSION 0.0.1)

add_compile_options(-Wall -Werror)

add_library(rpi-cam-inc INTERFACE)

target_include_directories(rpi-cam-inc INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(src)

option(RPI_CAM_TESTING OFF)
if (RPI_CAM_TESTING)
    add_subdirectory(test)
endif()

