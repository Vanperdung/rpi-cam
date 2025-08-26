# Typical flow to access video devce in userspace

1. Open the camera device

2. Query the camera's capabilites

- Purpose:

    * Get the bus info, version number of driver, device name.
    * Get the capabilities of a video device.

- Synopsis:

    ```C++
    int ioctl(int fd, VIDIOC_QUERYCAP, struct v4l2_capability *argp)
    ```

- `struct v4l2_capability`:

    * `_u8 driver[16]`: Name of driver.
    * `_u32 version`: Verion of driver.
    * `_u32 device_caps`: Device's capabilies.

3. Enumerate supported image formats

- Purpose: 

    * Identify which image formats that the video device supports for the buffer type that you intend to use.
    * Return error code if there is no support image format with this buffer type or there is no our desired image format.

- Synopsis: 

    ```C++
    int ioctl(int fd, VIDIOC_ENUM_FMT, struct v4l2_fmtdesc *argp)
    ```

- `struct v4l2_fmtdesc`: 

    * `_u32 type`: your desired buffer type need to be checked, to be filled by application.
    * `_u8 description[32]`: description of the format ("YUV 4:2:2" for example).
    * `_u32 pixelformat`: image identifier, this is a four character code as computed by the `v4l2_fourcc()`.

4. Set your desired format

- Purpose:

    * After ensuring that your desired format is available with that buffer type, we can set it using VIDIOC_S_FMT or VIDIOC_TRY_FMT.
    * Tells the driver which pixel format to use (e.g YUYV, NV12, MJPEG).
    * What resolution you want (e.g 1920x1080).

- Synopsis:

    ```C++
    int ioctl(int fd, VIDIOC_S_FMT, struct v4l2_format *argp)
    ```

    ```C++
    int ioctl(int fd, VIDIOC_TRY_FMT, struct v4l2_format *argp)
    ```

- Description:

    * VIDIOC_S_FMT:

        * The new format can be checked by driver and might be adjusted to match hardware limits.
        * Returns the final format (same as VIDIOC_G_FMT).
        * Return EBUSY if streaming is already active, EINVAL if type is invalid.

    * VIDIOC_TRY_FMT:

        * Doesn't change the device state (means the format is not applied)
        * Can be called anytime (never return EBUSY).
        * Is used when you're unsure about the new image format, it will validate a new image format, the driver may change the given values if they are not supported. The application should then check what is okay.

- `struct v4l2_format`:

    * `_u32 type`: your buffer type.
    * `struct v4l2_pix_format`: when type is V4L2_BUF_TYPE_VIDEO_CAPTURE and V4L2_BUF_TYPE_VIDEO_OUTPUT.
    * `struct v4l2_pix_format_mplane`: when type is V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE and V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE.

5. Request buffers

- Purpose: 

    * Requests the kernel component (vb2 or driver, idk) to allocate the memory for a number of buffer by using VIDIOC_REQBUFS.
    * Note that with DMABUF, buffers are located by the application, so the memory has not allocated yet when after calling this ioctl.
    * There is no buffer queued after this step.
    * Must check the returned value to identify the number of frames were created.

- Synopsis:

    ```C++
    int ioctl(int fd, VIDIOC_REQBUFS, struct v4l2_requestbuffer *argp)
    ```

- Description:

    * To allocate device buffers, application fills all necessary fields:
        
        * `count`: number of buffers that you want to have. Note that the vb2 may allocate the number of buffers that is smaller than you're requesting.
        * `type`: used to specify the type for the buffer, `enum v4l2_buf_type`.
        * `memory`: defines how the buffers are accessed/processed from the application, it can be V4L2_MEMORY_MMAP, V4L2_MEMORY_DMABUF, V4L2_MEMORY_USERPTR.

- [enum v4l2_buf_type](https://linuxtv.org/downloads/v4l-dvb-apis/userspace-api/v4l/buffer.html#c.V4L.v4l2_buf_type)

6. Import DMA Buffer (If we're using V4L2_MEMORY_DMABUF)

- Purpose: 

    * Decide where the actual memory for the buffers comes from and how it will be shared with the video driver.
    * There are 2 main approaches: Allocating buffers from another subsystem and Exporting existing driver buffers.

    A. Allocating buffers from another subsubsystem (DRM, GBM, ION or libcamera's buffer allocator)

        * Who allocates the memory for the buffers ?

            * Not the camera driver, not vb2 or anything else located in V4L2. Instead, some other subsystem allocates the memory.
            * This memory must:
                * Be accessible and shareable via DMA-BUF file descriptor for the camera driver.

        * How does camera driver use this memory ?

            * You pass the DMA-BUF to the camera driver use this memory.
            * The camera driver maps that FD for DMA (via `dma_buf_attach()` or something else) so the camera can write into it.
            * The image data is written directly to the memory via DMA => No copy happens => Zero-copy
    
    B. Exporting existing driver buffers.

        * Using `VIDIOC_EXPBUF` ioctl to import the buffer allocated by the camera driver.
        * Allow you to get the DMA fd for each those allocated buffers from camera driver.
        * Usually use with MMAP memory of `VIDIOC_REQBUF`.

- Synopsis: 

    ```C++
    int ioctl(int fd, VIDIOC_EXPBUF, struct v4l2_exportbuffer *argp)
    ```

- Description:

    * `type`: is filled by the application, the value must be similar to the buffer type as was previously used with VIDIOC_REQBUF. 
    * `index`: the index of the requested buffer. Valid index numbers range from 0 to the number of buffers allocated with VIDIOC_REQBUF minus 0.
    * For multi-planar, the `plane` field must be set to the index of plane to be exported. Otherwise, for the single-planar, the `plane` must be 0.
    * For multiplanr, every plane is exported separately using multiple VIDIOC_EXPBUF calls.
    * After calling VIDIOC_EXPBUF, `fd` field will be set by the driver. This is a DMA fd. It is recommended to close a DMA fd when it's no longer used to allow the associated memory to be reclaimed.

7. Queue all buffer

- Purpose:

    * To queue an empty (capturing) buffer or filled (output) buffer in the driver's incoming queue.
    * Queue each buffer each a time => all planes of a buffer is queued as well.

- Synopsis:

```C++
int ioctl(int fd, VIDIOC_QBUF, struct v4l2_buffer *argp)
```

- Description:

    * The semantics to queue buffer depends on the select I/O method, USERPTR, MMAP or DMABUF.
    * The `type` must be similar to the buffer type as was previously used with VIDIOC_REQBUF.
    * `index` is the index of buffer that is being enqueued.
    * When the buffer is intended for output (`type` is `V4L2_BUF_TYPE_VIDEO_OUTPUT`, `V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE`, `V4L2_BUF_TYPE_VBI_OUTPUT`) application must also initialize the `byteused`, `field` and the `timestamp` fields.
    * When using multi-planar API, the `m.planes` field must contain a userspace pointer to a filled-in array of `struct v4l2_plane` and the `length` field must be set to the number of elements in that array.
    * To enqueue a memory mapped buffer, application sets the `memory` field to `V4L2_MEMORY_MMAP`. When VIDIOC_QBUF is called with a pointer to this structure, the driver sets the `V4L2_BUF_FLAG_MAPPED` and `V4L2_BUF_FLAG_QUEUED` flags and clears the `V4L2_BUF_FLAG_DONE` flag in the `flags` field. 
    * To enqueue a DMABUF buffer, application sets the `memory` field to `V4L2_MEMORY_DMABUF` and the `m.fd` to the DMA fd assiociated with a DMABUF buffer. When multiplanar is used, the `m.planes` is used to contain the address of array of `struct v4l2_plane`, we must polulate the `m.fd` of the plane element. When VIDIOC_QBUF is called with a pointer to this structure, the driver sets the `V4L2_BUF_FLAG_QUEUED` flag and clears the `V4L2_BUF_FLAG_DONE` and `V4L2_MEMORY_MMAP` flags in the `flags` field. This ioctl locks the buffer. Locking a buffer means passing it to a driver for a hardware access (usually DMA). If an application accesses (read/write) a locked buffer then the result is undefined. Buffers remain locked until dequeued, until the VIDIOC_STREAMOFF or VIDIOC_REQBUF ioctls is called or until the device is closed.

8. Start/Stop streaming

- Purpose: 

    * To start/stop streaming I/O.

- Synopsis:

    ```C++
    int ioctl(int fd, VIDIOC_STREAMON, const int *argp)

    int ioctl(int fd, VIDIOC_STREAMOFF, const int *argp)
    ```

- Description:

    * The VIDIOC_STREAMON and VIDIOC_STREAMOFF ioctl start and stop the capture or ouput process during streaming I/O
    * Capture hardware is disabled there is no buffer are filled after stopping the streaming I/O until the VIDIOC_STREAMON is called.
    * Output hardware is disabled and no video signal is produced until VIDIOC_STREAMON is called.
    * Memory-to-memory devices will not start until VIDIOC_STREAMON is called.
    * The VIDIOC_STREAMOFF ioctl also unblocks any user pointer buffer, and removes all the buffers in the INPUT and OUTPUT queues. That means all images captured but not yet dequeued will be lost.

9. Dequeue buffers

- Purpose:

    * Application calles VIDIOC_DQBUF to dequeue a filled (capturing) or displayed (output) buffer from the driver's OUTPUT queue. They just set the `type`, `memory` and `reserved` fields of the `v4l2_buffer` as above. 
    * By default VIDIOC_DQBUF blocks when no buffer is in the OUTPUT queue. When O_NONBLOCK flag was given to the open() function, VIDIOC_DQBUF returns immediately with an EAGAIN error code when no buffer is available.

- Synopsis:

    ```C++
    int ioctl(int fd, VIDIOC_DQBUF, struct v4l2_buffer *argp)
    ```

10. Cleanup            

    - Stop streaming.
    - Close exported DMA fd (if DMA was used).
    - Release buffer (VIDIOC_REQBUFS with count = 0)
    - Unmap Buffer (if MMAP was used)
    - Close the video device.