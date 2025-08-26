# Single plane - Multiple plane

To identify the current buffer is single plane or multiple plane

    1. Call VIDIOC_QUERYCAP to query the capabilities of video device.
        
        * Look for `*_MPLANE`, if it's available, the video device support multiple plane mode.

    2. Enumerate the image formats that are supported by the video device by using VIDIOC_ENUM_FMT

        * Verify that with our desired buffer type (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE for example), what image formats are supported by video device.