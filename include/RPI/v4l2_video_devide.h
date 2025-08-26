#pragma once

#include <string>

#include <RPI/status.h>

namespace RPI
{

class V4L2VideoDevice
{
    using u32 = unsigned int;
public:
    V4L2VideoDevice();
    virtual ~V4L2VideoDevice();

    eStatus init(std::string node = {});
    eStatus exit();

    u32 caps();

private:
    eStatus verifyVideoDev();

    std::string video_node_;
};

} // namespace RPi