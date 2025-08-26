add_library(rpi-cam-common SHARED
    log.cpp
    unique_fd.cpp
    shared_fd.cpp
)

target_link_libraries(rpi-cam-common PRIVATE
    rpi-cam-inc
)