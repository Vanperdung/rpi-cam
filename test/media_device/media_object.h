#pragma once

class MediaDevice;

class MediaObject
{
public:
    MediaObject(MediaDevice *media_device = nullptr, unsigned int id = 0);
    virtual ~MediaObject() = default;

    unsigned int id() { return id_; }

protected:
    MediaDevice *media_device_;
    unsigned int id_;
};