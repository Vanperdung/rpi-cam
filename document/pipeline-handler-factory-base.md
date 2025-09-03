- PipelineHandlerFactory inherits PipelineHandlerFactoryBase.

- PipelineHandlerFactoryBase has:

    * create() method: which is used to create instance of subclasses of PipelineHandler. It calles the createInstance() method, which is the actual method to create instance of subclasses of PipelineHandler. 

        ```C++
        std::shared_ptr<PipelineHandler> create(CameraManager *manager) const;
        ```
    
    * createInstance() method: subclasses of PipelineHandlerFactoryBase override this pure virtual function.

    * each subclass of PipelineHandlerFactoryBase has `name_` member used to find the corresponding PipelineHandlerFactory instance. The list of PipelineHandlerFactory instances are stored in a static vector:

        ```C++
        /**
        * \brief Retrieve the list of all pipeline handler factories
        * \return the list of pipeline handler factories
        */
        std::vector<PipelineHandlerFactoryBase *> &PipelineHandlerFactoryBase::factories()
        {
            /*
            * The static factories map is defined inside the function to ensure
            * it gets initialized on first use, without any dependency on
            * link order.
            */
            static std::vector<PipelineHandlerFactoryBase *> factories;
            return factories;
        }
        ```

- When calling REGISTER_PIPELINE_HANDLER() with your PipelineHandler subclass and the name to identify its factory, a static global factory of PipelineHandlerFactory is created. Its address is stored while constructing to the static vector mentioned above.