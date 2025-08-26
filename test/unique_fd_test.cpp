#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>

#include <RPI/unique_fd.h>
#include <RPI/log.h>

#include "test.h"

using namespace RPI;

class UniqueFdTest : public Test
{
public:
    eStatus init() override
    {
        fd_ = ::open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
        if (fd_ < 0)
            return RPI_FAILED;

        struct stat s;
        if (fstat(fd_, &s))
            return RPI_FAILED;
        
        inode_num_ = s.st_ino;

        return RPI_SUCCESS;
    }

    eStatus run() override
    {
        UniqueFd unique_fd;
        
        if (unique_fd.get() != -1 || unique_fd.isValid() == true)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: Empty UniqueFd must be invalid");
            return RPI_FAILED;
        }

        UniqueFd unique_fd2(fd_);
        if (unique_fd2.get() != fd_ || unique_fd2.isValid() == false)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: UniqueFd must be identical with fd_");
            return RPI_FAILED;
        }
        if (isValid(unique_fd2.get()) == false)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: inode number must be identical with fd_");
            return RPI_FAILED;
        }

        UniqueFd unique_fd3 = std::move(unique_fd2);
        if (unique_fd3.get() != fd_ || unique_fd3.isValid() == false)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: UniqueFd must be identical with fd_");
            return RPI_FAILED;
        }
        if (isValid(unique_fd3.get()) == false)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: inode number must be identical with fd_");
            return RPI_FAILED;
        }
        if (unique_fd2.get() != -1 || unique_fd2.isValid() == true)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: Moved UniqueFd must be invalid");
            return RPI_FAILED;
        }
        if (isValid(unique_fd2.get()) == true)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: Empty UniqueFd must be invalid");
            return RPI_FAILED;
        }

        {
            UniqueFd unique_fd4 = std::move(unique_fd3);
        }

        if (isValid(fd_) == true)
        {
            LOGGING(RPI_LOG_ERROR, "Failed: Closed fd must be invalid");
            return RPI_FAILED;
        }

        fd_ = -1;

        LOGGING(RPI_LOG_INFO, "Success");
        return RPI_SUCCESS;
    }

    eStatus cleanup() override
    {
        if (fd_ >= 0)
            ::close(fd_);

        return RPI_SUCCESS;
    }

    bool isValid(int fd)
    {
        struct stat s;
        if (::fstat(fd, &s))
            return false;
        
        return s.st_ino == inode_num_;
    }

private:
    int fd_;
    ino_t inode_num_;
};

TEST_REGISTER(UniqueFdTest);