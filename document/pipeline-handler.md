Pipeline handler is responsible for:

- detecting and registering camera devices available in the system.
- configuring the image acquisition and processing pipeline.
- starting and stopping the image acquisition and processing sessions.
- applying configuration settings requested by application and computed by image processing algo to the hardware devices.
- notifying applications of the availability of new images and delivering them to the correct locations.

## Create a PipelineHandler

```C++
class PipelineHandlerVivid : public PiplelineHandler
{
public:
    PipelineHandlerVivid(CameraManager *manager);
    CameraConfiguration *generateConfiguration(Camera *camera, Span<const StreamRole> roles) override;
    int configure(Camera *camera, CameraConfiguration *config) override;
    int exportFrameBuffers(Camera *camera, Stream *stream, std::vector<std::unique_ptr<FrameBuffer>> *buffers) override;
    int start(Camera *camera, const ControlList *controls) override;
    void stopDevice(Camera *camera) override;
    int queueRequestDevice(Camera *camera, Request *request) override;
    bool match(DeviceEnumerator *enumerator) override;
};
```
The PipelineHandle subclass must be registered with the pipeline handler factory by using the REGISTER_PIPELINE_HANDLER() macro:

```C++
#define REGISTER_PIPELINE_HANDLER(handler, name) \ 
    static PipelineHandlerFactory<handler> global_##handler##Factory(name);
```

```C++
REGISTER_PIPELINE_HANDLER(PipelineHandlerVivid, "vivid")
~ static PipelineHandlerFactory<PipelineHandlerVivid> global_PipelineHandlerVividFactory("vivid")
```

Read [pipeline-handler-factory-base.md](./pipeline-handler-factory-base.md) for more details about the usage of factory design pattern for PipeplineHandler implementation.

## Matching devices

The main entry point of a pipeline handler is the `PipelineHandler::match()` class member function. When the `CameraManager` is started (using the `CameraManager::start()`), all the registered PipelineHandler are iterated and their `match` function called with an enumerator of all devices it found on a system.