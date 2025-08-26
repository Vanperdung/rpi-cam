# Buffers

## Introduction

- Buffer contains the image data exchanged by the application and driver using one of the streaming I/O methods.

- In multi-planar, the data is held in plane, the buffer acts as a container of a group for the planes.

- Only pointers to the data is exchanged, the data itself is not copied.

## Interaction between controls, formats and buffers

- Buffer layout refers to all the information that we need to interpret the image data in the buffer, which are: pixel, format, stride, rotation, tiling, etc.

- `Formats` and `Controls` can affect the buffer layout.

- If a control can change the buffer layout, it's marked with `V4L2_CRTL_FLAG_MODIFY_LAYOUT`. 

- Changing format or controls that affect buffer size/layout requires the stream to be stopped. If you try to change them during streaming, the ioctl return EBUSY and control is marked as GRABBED until the stream stops.

- Typical sequence to change format/controls is:

    1. VIDIOC_STREAMOFF
    2. VIDIOC_REQBUFS(0)
    3. VIDIOC_S_EXT_CTRLS           # Set controls
    4. VIDIOC_S_FMT                 # Set formats
    5. VIDIOC_REQBUFS(n)            # Allocate buffers with new size
    6. VIDIOC_QBUF
    7. VIDIOC_STREAMON
