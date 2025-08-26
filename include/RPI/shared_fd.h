#pragma once

#include <memory>

namespace RPI
{

class SharedFd final
{
public:
    explicit SharedFd(const int &fd = -1);
    explicit SharedFd(int &&fd);
    SharedFd(const SharedFd &other);
    SharedFd(SharedFd &&other);
    ~SharedFd();

    SharedFd &operator=(const SharedFd &other);
    SharedFd &operator=(SharedFd &&other);

    int get() const { return fd_ ? fd_->fd() : -1; }
    bool isValid() const { return fd_ != nullptr; }

private:
    class Descriptor
    {
    public:
        Descriptor(int fd, bool dup);
        ~Descriptor();

        int fd() const { return fd_; }

    private:
        int fd_;
    };

    std::shared_ptr<Descriptor> fd_;
};

static inline bool operator==(const SharedFd &lhs, const SharedFd &rhs)
{
    return lhs.get() == rhs.get();
}

static inline bool operator!=(const SharedFd &lhs, const SharedFd &rhs)
{
    return lhs.get() != rhs.get();
}

} // namespace RPI