#include "media_object.h"
#include "media_entity.h"
#include "media_pad.h"

MediaPad::MediaPad(MediaDevice *media_device, 
                    const struct media_v2_pad *media_pad,
                    MediaEntity *entity)
    : MediaObject(media_device, media_pad->id),
    entity_(entity),
    index_(media_pad->index)
{
}

MediaPad::~MediaPad()
{
}

