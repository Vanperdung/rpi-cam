#include <string>

#include "media_device.h"

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