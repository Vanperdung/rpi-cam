#include <unistd.h>
#include <errno.h>

#include <cstring>

#include <RPI/shared_fd.h>
#include <RPI/log.h>

using namespace RPI;

/**
 * @brief Constuctor for ShareFd, copying a given fd
 * 
 * A SharedFd is created by duplicating the original fd. The original fd is
 * left untouched. The duplicated file descriptor will be closed automatically
 * when the all ShareFd instances that own it are destroyed
 * 
 * @param fd The file descriptor is being copied
 */
SharedFd::SharedFd(const int &fd)
{
    if (fd < 0)
        return;

    fd_ = std::make_shared<Descriptor>(fd, true);
    if (fd_->fd() < 0)
        fd_.reset();
}

/**
 * @brief Constuctor for ShareFd, taking the ownership of a given fd
 * 
 * A SharedFd is created by taking the ownership of the original fd. After this
 * constructor, the original must not be touched by the caller anymore. The
 * taken file descriptor will be closed automatically when all ShareFd instances
 * that own it are destroyed.
 * 
 * @param fd The file descriptor is being copied
 */
SharedFd::SharedFd(int &&fd)
{
    if (fd < 0)
        return;

    fd_ = std::make_shared<Descriptor>(fd, false);
    if (fd_->fd() < 0)
        fd_.reset();

    fd = -1;
}

/**
 * @brief
 */
SharedFd::SharedFd(const SharedFd &other)
    : fd_(other.fd_)
{
}

/**
 * @brief
 */
SharedFd::SharedFd(SharedFd &&other)
    : fd_(std::move(other.fd_))
{
}

/**
 * @brief
 */
SharedFd::~SharedFd()
{
}

/**
 * @brief
 */
SharedFd &SharedFd::operator=(const SharedFd &other)
{
    fd_ = other.fd_;

    return *this;
}

/**
 * @brief
 */
SharedFd &SharedFd::operator=(SharedFd &&other)
{
    fd_ = std::move(other.fd_);

    return *this;
}

/**
 * @brief
 */
SharedFd::Descriptor::Descriptor(int fd, bool dup)
{
    if (dup == false)
    {
        fd_ = fd;
        return;
    }

    fd_ = ::dup(fd);
    if (fd_ == -1)
    {
        LOGGING(RPI_LOG_ERROR, "Failed to dup() fd: %s", strerror(errno));
        return;
    }
}

/**
 * @brief
 */
SharedFd::Descriptor::~Descriptor()
{
    if (fd_ >= 0)
        ::close(fd_);
}