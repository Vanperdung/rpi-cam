- Image format negotiation:

    * is the process to ensure that the camera driver and application agree on the same:

        * pixel format: defines how each pixel data is represented (YUYV, NV12, RGB, ...).
        * resolution & frame size: defines width, height and the size for each frame (buffer). 
        * colorspace: defines how to interpret the pixel data to real-world color (sRGB, Rec.709, ...).
        * stride & alignment: defines how many bytes per row and any padding between lines or planes. 
    
    * happens by using VIDIOC_TRY_FMT/VIDIOC_S_FMT.