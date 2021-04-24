#if !defined(__BLETASK_H__)
#define __BLETASK_H__

#include <sample.h>
#include <Task.h>
#include <BufferQueue.h>
#include <BleAdapter.h>

class BleTask : public Task
{
private:
    BleAdapter &_bleAdapter;
    BufferQueue &_sourceQueue;

public:
    BleTask(BleAdapter &bleAdapter, BufferQueue &sourceQueue)
        : _bleAdapter(bleAdapter), _sourceQueue(sourceQueue)
    {
    }

protected:
    virtual void run()
    {
        size_t bufferSize = 512;
        sample_t buffer[bufferSize];

        _bleAdapter.setUp();

        while (!stopping())
        {
            _sourceQueue.read(buffer, bufferSize);

            _bleAdapter.write(buffer, bufferSize);

            delay(1000);
            Serial.println("BleTask::run()");
        }

        _bleAdapter.tearDown();
    }
};

#endif // __BLETASK_H__
