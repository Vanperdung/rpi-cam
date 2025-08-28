#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/dma-heap.h>

int main(int argc, char *argv[])
{
    int heap_fd = ::open("/dev/dma_heap/linux,cma", O_RDWR);
    if (heap_fd < 0)
        return -1;

    // Allocate 8 MB contiguous memory
    struct dma_heap_allocation_data data = {
        .len = 8 * 1024 * 1024,
        .fd_flags = O_CLOEXEC | O_RDWR,
    };

    if (::ioctl(heap_fd, DMA_HEAP_IOCTL_ALLOC, &data) < 0)
    {
        ::close(heap_fd);
        return -1;
    }

    int dma_fd = data.fd;

    ::close(heap_fd);
    
    void *p = ::mmap(NULL, data.len, PROT_READ | PROT_WRITE, MAP_SHARED, dma_fd, 0);
    if (p == MAP_FAILED)
    {
        ::close(dma_fd);
        return -1;
    }

    ::munmap(p, data.len);
    ::close(dma_fd);

    return 0;
}