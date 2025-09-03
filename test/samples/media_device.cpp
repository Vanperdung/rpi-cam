#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/media.h>

#include <string>

#include <RPI/unique_fd.h>
#include <RPI/log.h>

class MediaDevice
{
public:
    MediaDevice();
    virtual ~MediaDevice();

    int init(std::string media_node);
    int cleanup();

private:
    int getInfo();

    RPI::UniqueFd fd_;
    std::string name_;
};

MediaDevice::MediaDevice()
{

}

MediaDevice::~MediaDevice()
{
    
}

int MediaDevice::init(std::string media_node)
{
    if (media_node.empty())
        return -1;

    if (fd_.isValid())
        return -1;
    
    name_ = media_node;
    
    int fd = ::open(name_.c_str(), O_RDWR);
    if (fd < 0)
        return -1;

    fd_ = std::move(RPI::UniqueFd(fd));
    if (fd_.get() != fd)
    {
        ::close(fd);
        return -1;
    }

    if (getInfo() < 0)
        return -1;

    return 0;
}

int MediaDevice::cleanup()
{

    return 0;
} 

int MediaDevice::getInfo()
{
    if (!fd_.isValid())
        return -1;
    
    struct media_device_info media_info;
    int ret = ::ioctl(fd_.get(), MEDIA_IOC_DEVICE_INFO, &media_info);
    if (ret < 0)
        return -1;

    RPI::LOGGING(RPI::RPI_LOG_INFO, "%s-%s-%s-%s", media_info.driver,
                                                media_info.model,
                                                media_info.serial,
                                                media_info.bus_info);

    return 0;
}

int main(int argc, char *argv[])
{
    std::string media_node;
    int ret = -1;

    media_node = (argc == 1) ? "/dev/media0" : argv[1];

    MediaDevice media_dev;
    ret = media_dev.init(media_node);
    if (ret < 0)
        return ret;

    return ret;
}

