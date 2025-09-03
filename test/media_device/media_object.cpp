#include "media_device.h"
#include "media_object.h"

MediaObject::MediaObject(MediaDevice *media_device, unsigned int id)
    : media_device_(media_device), id_(id)
{
}

