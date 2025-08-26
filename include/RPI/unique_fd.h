#pragma once

namespace RPI
{

class UniqueFd final
{
public:
    explicit UniqueFd(int fd = -1);
    UniqueFd(UniqueFd &&other);
    UniqueFd(const UniqueFd &other) = delete;
    ~UniqueFd();

    [[nodiscard]] int release();
    void reset(int fd = -1);
    bool isValid() { return fd_ >= 0; }
    void swap(UniqueFd &other);
    int get() const { return fd_; }

    UniqueFd &operator=(const UniqueFd &other) = delete;

private:
    int fd_;
};

} // namespace RPI