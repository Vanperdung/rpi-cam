#include <unistd.h>

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
    int temp_fd = fd_;
    
    fd_ = fd;

    if (temp_fd > 0)
        ::close(temp_fd);
}

void UniqueFd::swap(UniqueFd &other)
{
    int temp_fd = -1;

    temp_fd = fd_;
    fd_ = other.fd_;
    other.fd_ = temp_fd;
}