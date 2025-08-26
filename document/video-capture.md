# Video Capture Interface

## Introduction

- Video capture device samples an analog video signal and stores the digitized images in memory.

- Today nearly all video capture devices can capture at > 30 frames per second.

## Querying Capabilities

- If the device supports video capture interface, the `capabilities` flag should have `V4L2_CAP_VIDEO_CAPTURE` and `V4L2_CAP_VIDEO_CAPTURE_MPLANE` flags, which is returned by the VIDIOC_QUERYCAP ioctl.

## Image Format Negotiation

- Capture result depends on:

    * Cropping: Defines the area of the sensor/image to capture.
    * Image format: Defines how the data is stored in memory (RGB/YUV, bits per pixel, width, height).
    * Together they also define how the images are scaled in the process.

- The camera parameters are not reset when the video device is opened by `open()`, so the application must explicitly set what they need.

- Query current format: Filling the `type` in the `struct v4l2_format` with your desired buffer type then calling VIDIOC_G_FMT ioctl. The driver will return the current setting in `v4l2_pix_format` or `v4l2_pix_format_mplane`.

- Request new format: Using VIDIOC_S_FMT ioctl.

- 