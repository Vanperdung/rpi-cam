#pragma once

#include <linux/media.h>

#include "media_object.h"

class MediaEntity;
class MediaDevice;

class MediaPad final : public MediaObject
{
public:
    MediaPad(MediaDevice *media_device, 
                const struct media_v2_pad *media_pad,
                MediaEntity *entity);
    ~MediaPad();

private:
    MediaEntity *entity_; /* The owner of this MediaPad object */
    unsigned int index_;
};
