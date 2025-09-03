#include <linux/media.h>

#include <string>
#include <unordered_map>

#include <RPI/unique_fd.h>

class MediaObject;
class MediaEntity;

class MediaDevice final
{
public:
    MediaDevice();
    ~MediaDevice();

    int init(std::string media_node);
    int cleanup();

    std::string name() 
    { return name_; }

    std::string driver()
    { return driver_; }

    std::string model()
    { return model_; }

    unsigned int version()
    { return version_; }

    unsigned int hwRevision()
    { return hwRevision_; }

    MediaObject *object(unsigned int id);

    int enumerate();

private:
    int getInfo();
    int populateEntites(const struct media_v2_topology &media_topology);
    int populatePads(const struct media_v2_topology &media_topology);
    int populateLinks(const struct media_v2_topology &media_topology);
    struct media_v2_interface *findInterface(const struct media_v2_topology &media_topology,
                                unsigned int media_entity_id);
    int addObject(MediaObject *object);

    RPI::UniqueFd fd_;
    std::string name_;
    std::string driver_; /* Name of driver implementing the media API */
    std::string model_; /* Device model name */
    unsigned int version_; /* Media API version, formatted by KERNEL_VERSION() */
    unsigned int hwRevision_; /* Hardware device revision in a driver-specific format */
    std::unordered_map<unsigned int, MediaObject*> objects_;
};
