#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/media.h>

#include <string>

#include <RPI/log.h>

#include "media_device.h"
#include "media_object.h"
#include "media_entity.h"
#include "media_pad.h"

MediaDevice::MediaDevice()
{

}

MediaDevice::~MediaDevice()
{
    cleanup();
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
    std::unordered_map<unsigned int, MediaObject*>::iterator it;

    for (it = objects_.begin(); it != objects_.end(); it++)
    {
        delete it->second;
    }

    return 0;
} 

int MediaDevice::enumerate()
{
    struct media_v2_topology media_topology = {};
    struct media_v2_entity *media_entities = nullptr;
    struct media_v2_interface *media_interfaces = nullptr;
    struct media_v2_pad *media_pads = nullptr;
    struct media_v2_link *media_links = nullptr;
    unsigned long long version;

    if (!fd_.isValid())
        return -1;

    int ret = ::ioctl(fd_.get(), MEDIA_IOC_G_TOPOLOGY, &media_topology);
    if (ret < 0)
        return -1;

    media_entities = new struct media_v2_entity[media_topology.num_entities]();
    media_interfaces = new struct media_v2_interface[media_topology.num_interfaces]();
    media_pads = new struct media_v2_pad[media_topology.num_pads]();
    media_links = new struct media_v2_link[media_topology.num_links]();

    media_topology.ptr_entities = reinterpret_cast<unsigned long long>(media_entities);
    media_topology.ptr_interfaces = reinterpret_cast<unsigned long long>(media_interfaces);
    media_topology.ptr_pads = reinterpret_cast<unsigned long long>(media_pads);
    media_topology.ptr_links = reinterpret_cast<unsigned long long>(media_links);
    
    version = media_topology.topology_version;

    /**
     * Keep calling MEDIA_IOC_G_TOPOLOGY again to get the properties of entities,
     * interfaces, pads, links.
     */
    ret = ::ioctl(fd_.get(), MEDIA_IOC_G_TOPOLOGY, &media_topology);
    if (ret < 0 || media_topology.topology_version != version)
    {
        ret = -1;
        goto tail;
    }

    if ((populateEntites(media_topology) && 
            populatePads(media_topology) &&
            populateLinks(media_topology)) != 0)
    {
        ret = -1;
        goto tail;
    }



    return ret;
tail:
    delete[] media_entities;
    delete[] media_interfaces;
    delete[] media_pads;
    delete[] media_links;

    return ret;
}

MediaObject *MediaDevice::object(unsigned int id)
{
    if (objects_.find(id) == objects_.end())
        return nullptr;
    
    return objects_.at(id);
}

int MediaDevice::getInfo()
{
    if (!fd_.isValid())
        return -1;
    
    struct media_device_info media_info;
    int ret = ::ioctl(fd_.get(), MEDIA_IOC_DEVICE_INFO, &media_info);
    if (ret < 0)
        return -1;

    driver_ = media_info.driver;
    model_ = media_info.model;
    version_ = media_info.media_version;
    hwRevision_ = media_info.hw_revision;
    
    return 0;
}

int MediaDevice::populateEntites(const struct media_v2_topology &media_topology)
{
    struct media_v2_entity *media_entities = 
        reinterpret_cast<struct media_v2_entity*>(media_topology.ptr_entities);

    if (media_entities == nullptr)
        return -1;

    for (unsigned int i = 0; i < media_topology.num_entities; i++)
    {
        if (!MEDIA_V2_ENTITY_HAS_FLAGS(version_))
            return -1;
        
        struct media_v2_interface *media_interface = findInterface(media_topology, media_entities[i].id);
        MediaObject *entity = new MediaEntity(this, &media_entities[i], media_interface);

        /**
         * MediaDevice keeps all the MediaEntity objects.
         */
        if (addObject(entity) != 0)
        {
            LOGGING(RPI::RPI_LOG_ERROR, "Failed to add MediaEntity object, id=%d", entity->id());
            delete entity;
            return -1;
        }
    }

    return 0;
}

int MediaDevice::populatePads(const struct media_v2_topology &media_topology)
{
    struct media_v2_pad *media_pads = 
        reinterpret_cast<struct media_v2_pad*>(media_topology.ptr_pads);

    if (media_pads == nullptr)
        return -1;

    for (unsigned int i = 0; i < media_topology.num_pads; i++)
    {
        MediaEntity *entity = dynamic_cast<MediaEntity*>(object(media_pads[i].entity_id));
        MediaObject *pad = new MediaPad(this, &media_pads[i], entity);
        
        if (addObject(pad) < 0)
            return -1;
    }

    return 0;
}

int MediaDevice::populateLinks(const struct media_v2_topology &media_topology)
{
    return 0;
}

struct media_v2_interface *MediaDevice::findInterface(const struct media_v2_topology &media_topology,
                                unsigned int media_entity_id)
{
    struct media_v2_link *media_links = reinterpret_cast<struct media_v2_link*>(media_topology.ptr_links);
    unsigned int media_interface_id = 0;
    unsigned int i;

    for (i = 0; i < media_topology.num_links; i++)
    {
        if (media_links[i].sink_id == media_entity_id && media_links[i].flags & MEDIA_LNK_FL_INTERFACE_LINK)
        {
            media_interface_id = media_links[i].source_id;
            break;
        }    
    }

    if (i == media_topology.num_links)
        return nullptr;
    
    struct media_v2_interface *media_interface = 
        reinterpret_cast<struct media_v2_interface*>(media_topology.ptr_interfaces);

    for (i = 0; i < media_topology.num_interfaces; i++)
    {
        if (media_interface[i].id == media_interface_id)
            return &media_interface[i];
    }

    return nullptr;
}

int MediaDevice::addObject(MediaObject *object)
{
    /**
     * Make sure that the entity does not exist.
     */
    if (objects_.find(object->id()) != objects_.end())
        return -1;

    objects_[object->id()] = object;
    return 0;
}