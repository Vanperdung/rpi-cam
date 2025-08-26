#include <unistd.h>

#include <utility>

#include <RPI/unique_fd.h>

using namespace RPI;

UniqueFd::UniqueFd(int fd)
    : fd_(fd)
{
}

UniqueFd::UniqueFd(UniqueFd &&other)
    : fd_(other.release())
{
}

UniqueFd::~UniqueFd()
{
    reset();
}

int UniqueFd::release()
{
    int fd = fd_;

    fd_ = -1;
    return fd;
}

void UniqueFd::reset(int fd)
{
    if (fd == fd_)
        return;
    
    int temp_fd = std::exchange(fd_, fd);
    if (temp_fd >= 0)
        ::close(temp_fd);
}

void UniqueFd::swap(UniqueFd &other)
{
    std::exchange(fd_, other.fd_);
}