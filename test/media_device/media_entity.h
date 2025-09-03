#pragma once

#include <string>
#include <vector>

#include <linux/media.h>

#include "media_object.h"

class MediaPad;

class MediaEntity final : public MediaObject
{
public:
    /**
    * enum Type is the type of interface exposed by the entity to the userspace.
    */
    enum class Type
    {
        Invalid,
        MediaEntity,
        V4L2Subdevice,
        V4L2VideoDevice,
    };

    MediaEntity(MediaDevice *media_device, 
                    const struct media_v2_entity *media_entity, 
                    const struct media_v2_interface *media_interface);
    ~MediaEntity() = default;

private:
    std::string name_;
    unsigned int function_;
    unsigned int flags_;
    Type type_;
    std::vector<MediaPad*> pads_;
}; 