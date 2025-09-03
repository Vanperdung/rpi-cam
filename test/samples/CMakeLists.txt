add_executable(cma_heap
    cma_heap.cpp
)

add_executable(system_heap
    system_heap.cpp
)

add_executable(media_device
    media_device.cpp
)

target_link_libraries(media_device PRIVATE
    rpi-cam-inc
    rpi-cam-common
)