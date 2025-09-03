add_executable(media_test
    main.cpp
    media_device.cpp
    media_object.cpp
    media_entity.cpp
    media_pad.cpp
)

target_link_libraries(media_test PRIVATE
    rpi-cam-inc
    rpi-cam-common
)