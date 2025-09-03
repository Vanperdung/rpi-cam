- DmaBufAllocator
    
    * a wrapper of dma-buf provider in user-space.
    * exposes functions to allocate dma-buffer.

- Different provider may provide dma-buffer with different properties.

- Which providers are acceptable is specified by the `type` argument passed to the DmaBufAllocator() constructor.

- DmaBufAllocatorFlag
    
    * type of the dma-buf provider.
    * has 3 different types:

        * `CmaHeap`: allocates from a CMA dma-heap, providing physically-contiguous memory.
        * `SystemHeap`: allocates from the system dma-heap, using the page allocator (not guaranteed contiguous).
        * `UDmaBuf`: allocates using a memfd + /dev/udmabuf.

    