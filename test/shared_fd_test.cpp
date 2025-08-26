#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <memory>

#include <RPI/shared_fd.h>
#include <RPI/status.h>
#include <RPI/log.h>
#include "test.h"

class SharedFdTest : public Test
{
public:
    eStatus init()
    {
        fd_ = ::open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
        if (fd_ < 0)
            return RPI_FAILED;
        
        /* Cache inode number of temp file */
        struct stat s;
        if (::fstat(fd_, &s))
            return RPI_FAILED;
        
        inode_ = s.st_ino;

        return RPI_SUCCESS;
    }

    eStatus run() override
    {
        /* Test creating empty SharedFd */
        sFd1_ = std::make_unique<SharedFd>();
        if (sFd1_->get() != -1)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (default should be -1)");
            return RPI_FAILED;
        }
        sFd1_.reset();

        /* Test creating SharedFd by copying numerical file descriptor*/
        sFd1_ = std::make_unique<SharedFd>(fd_);
        if (sFd1_->get() == fd_)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (fds can't be identical)");
            return RPI_FAILED;
        }
        if (fd_ == -1 || sFd1_->get() == -1)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (fds must not be -1)");
            return RPI_FAILED;
        }
        sFd1_.reset();

        int copy_fd = fd_;
        sFd1_ = std::make_unique<SharedFd>(std::move(fd_));

        if (sFd1_->get() != copy_fd)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (fd must be identical)");
            return RPI_FAILED;
        }
        if (fd_ != -1)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (original fd must be set to -1)");
            return RPI_FAILED;
        }

        sFd2_ = std::make_unique<SharedFd>();
        *sFd2_ = *sFd1_;
        if (*sFd1_ != *sFd2_)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (original fds must be identical)");
            return RPI_FAILED;
        }
        sFd2_.reset();

        sFd2_ = std::make_unique<SharedFd>(*sFd1_);
        if (*sFd1_ != *sFd2_)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (original fds must be identical)");
            return RPI_FAILED;
        }
        sFd2_.reset();

        sFd2_ = std::make_unique<SharedFd>(std::move(*sFd1_));
        if (sFd1_->get() != -1 || sFd1_->isValid() == true)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (original fd must be -1)");
            return RPI_FAILED;
        }

        if (sFd2_->get() < 0)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (new fd must be higher than 0)");
            return RPI_FAILED;
        }

        *sFd1_ = std::move(*sFd2_);
        if (sFd2_->get() != -1 || sFd2_->isValid() == true)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (original fd must be -1)");
            return RPI_FAILED;
        }

        if (sFd1_->get() < 0)
        {
            LOGGING(RPI_LOG_ERROR, "Failed fd check (new fd must be higher than 0)");
            return RPI_FAILED;
        }
        sFd1_.reset();
        sFd2_.reset();

        LOGGING(RPI_LOG_INFO, "Success");
        return RPI_SUCCESS;
    }

private:
    std::unique_ptr<SharedFd> sFd1_;
    std::unique_ptr<SharedFd> sFd2_;
    int fd_;
    ino_t inode_;
};

TEST_REGISTER(SharedFdTest);