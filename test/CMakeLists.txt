add_subdirectory(samples)

add_executable(shared_fd_test
    shared_fd_test.cpp
)

target_link_libraries(shared_fd_test PRIVATE
    rpi-cam-inc
    rpi-cam-common
)

add_executable(unique_fd_test
    unique_fd_test.cpp
)

target_link_libraries(unique_fd_test PRIVATE
    rpi-cam-inc
    rpi-cam-common
)