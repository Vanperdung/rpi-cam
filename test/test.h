#pragma once

#include <RPI/status.h>

using namespace RPI;

class Test
{
public:
    Test() = default;
    virtual ~Test() = default;

    eStatus execute()
    {
        eStatus status;

        status = init();
        if (status == RPI_FAILED)
            return status;
        
        status = run();
        if (status == RPI_FAILED)
            return status;
        
        status = cleanup();
        if (status == RPI_FAILED)
            return status;
        
        return RPI_SUCCESS;
    }

protected:
    virtual eStatus init() { return RPI_SUCCESS; }
    virtual eStatus run() = 0;
    virtual eStatus cleanup() { return RPI_SUCCESS; }
};

#define TEST_REGISTER(ConcreteTest) \
int main()                          \
{                                   \
    ConcreteTest test;              \
    return test.execute();          \
}
