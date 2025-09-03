- libcamera and VC4 (broadcom videocore 4) are related only through the display pipeline. Here is the relationship:

    * libcamera = image capture + image processing:

        * libcamera handles camera sensors, image acquisition, ISP (image signal processor), and image format negotiation.
        * the main job of libcamera is to capture the image via V4L2.
        * the image data will be stored in a memory, which are represented by framebuffer instance.
    
    * VC4 = display + GPU:

        * VC4 handles the image data and shows it on screen.
        * VC4 does not control the camera sensor and also capture the image data, it only knows about framebuffer instances and handles them.

- A brief diagram showing the interaction between libcamera and VC4:

    * camera sensor -> V4L2 -> libcamera -> memory (containing framebuffer) -> VC4 -> screen.