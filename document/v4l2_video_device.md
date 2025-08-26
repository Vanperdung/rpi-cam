# V4L2_VIDEO_DEVICE

## Purpose

- Does all interactions with video device. Other objects communicates with the camera device by using members/methods of this class's instance.

## Methods

- init():

    * checks the statistics of the video device.
    * opens the video device
    * gets the capabilities of the camera.
    * gets the type of video device.
    * gets the name of video device.  

- caps(): 

    * returns the supported capabilities of the video device.

- trySetFormat(set):

    * sets the format (width, height, pixelformat, field) for the video device.
    * if set is true, using VIDIOC_TRY_FMT, if not using VIDIOC_S_FMT.
    * without `*_MPLANE`: trySetFormatSinglePlane_().
    * with `*_MPLANE`: trySetFormatMultiPlane_().
    * if `*_META_*`: trySetFormatMeta_().

- trySetFormatSinglePlane_():

* 

- trySetFormatMultiPlane_():

* 

- getFormat():

    * gets the current format of the video device.
    * without `*_MPLANE`: getFormatSinglePlane_().
    * with `*_MPLANE`: getFormatMultiPlane_().
    * if `*_META_*`: getFormatMeta_().

- getFormatSinglePlane_():

    * 

- getFormatMultiPlane_():

    * 

- requestBuffer():

    * requests the kernel components (vb2 core, camera device driver) to allocate memory for a number of buffers, using VIDIOC_REQBUF.
    * without `*_MPLANE`: requestBufferSinglePlane_().
    * with `*_MPLANE`: requestBufferMultiPlane_().

- requestBufferSinglePlane_():

    * doesn't support multiplanes.

- requestBufferMultiPlane_():

    * supports multiplanes.

- queryBuffer():

    * is used to get the information of the buffers (and planes if `*_MPLANES`).
    * is used for `MMAP` type.
    * without `*_MPLANE`: queryBufferSinglePlane_().
    * with `*_MPLANE`: queryBufferMultiPlane_().

- queryBufferSinglePlane_():

    *

- queryBufferMultiPlane_():

    *

- importBuffer():

    * is used for `DMABUF` type.


- releaseBuffer():

    * requests deallocating the requested buffers, using VIDIOC_REQBUF with size = 0.
    * all queued buffer will be deleted.

- close():

    * 