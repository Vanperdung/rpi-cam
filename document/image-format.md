# Image format

A image format describes how the image data (which is stored in the buffer) is represented, stored, and interpreted.

To be more detailed:

- How pixel data is organized in memory (or in buffer).

- How many bits are used per pixel (bit depth).

- What color model is used (e.g RGB, YUV, Bayer).

- Whether the image is compressed or uncompressed.

## How pixel data is organized in memory (or in buffer) ? 

- Packed (Interleaved): All pixel components are stored together in sequence (e.g YUYV, RGB24).

- Planar: Components are stored in seperate planes (e.g Y plane, then U plane, then V plane in YUV420).

## How many bits are used per pixel (bit depth) ?

- Define how many bits are used to representeach channel (or pixel) (e.g 8-bit, 10-bit, 12-bit).

## What color model is used (e.g RGB, YUV, Bayer) ?

- Determine how the colors are represent.

- RGB

- YUV

- Bayer

- Monochrome

## Whether the image is compressed or uncompressed ? 

- Uncompressed: Every pixel is stored as raw data (e.g YUYV, RGB).

- Compressed: Data is reduced using a codec (e.g JPEG, H.264).