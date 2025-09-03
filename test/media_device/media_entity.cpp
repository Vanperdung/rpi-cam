#include "media_entity.h"

/**
 * @brief Construct a MediaEntity object
 * 
 * @param media_device The media device that MediaEntity object belongs to
 * @param media_entity The media entity data
 * @param interface The media interface data that MediaEntity object exposes
 */
MediaEntity::MediaEntity(MediaDevice *media_device, 
                    const struct media_v2_entity *media_entity, 
                    const struct media_v2_interface *media_interface)
    : MediaObject(media_device, media_entity->id),
    name_(media_entity->name), 
    function_(media_entity->function),
    flags_(media_entity->flags),
    type_(Type::MediaEntity) 
{
    if (media_interface == nullptr)
        return;

    switch (media_interface->intf_type)
    {
    case MEDIA_INTF_T_V4L_SUBDEV:
        type_ = Type::V4L2Subdevice;
        break;
    case MEDIA_INTF_T_V4L_VIDEO:
        type_ = Type::V4L2VideoDevice;
        break;
    default:
        type_ = Type::Invalid;
        break;
    }         
}