add_executable(shared_fd_test
    shared_fd_test.cpp
)

target_link_libraries(shared_fd_test PRIVATE
    rpi-cam-inc
    rpi-cam-common
)