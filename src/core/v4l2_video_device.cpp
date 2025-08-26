#include <sys/stat.h>

#include <string>

#include <RPI/v4l2_video_devide.h>
#include <RPI/log.h>

using namespace RPI;

#define V4L2_VIDEO_NODE    "/dev/video0"

V4L2VideoDevice::V4L2VideoDevice()
{

}

V4L2VideoDevice::~V4L2VideoDevice()
{

}

eStatus V4L2VideoDevice::init(std::string node)
{
    video_node_ = node.empty() ? V4L2_VIDEO_NODE : node;

    LOGGING(RPI_LOG_WARNING, "Using default video node: %s", video_node);

    if (verifyVideoDev() != RPI_SUCCESS)
        return RPI_INVALID;

    if (::open())
}

eStatus V4L2VideoDevice::verifyVideoDev()
{
    struct stat dev_stat;
    int ret = stat(video_node_.c_str(), &dev_stat);
    if (ret < 0)
    {
        LOGGING(RPI_LOG_ERROR, "%s doesn't exist", video_node_);
        return RPI_INVALID;
    }

    return RPI_SUCCESS;
}