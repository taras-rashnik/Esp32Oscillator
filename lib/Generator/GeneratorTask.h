#if !defined(__GENERATORTASK_H__)
#define __GENERATORTASK_H__

#include <sample.h>
#include <Task.h>
#include <Generator.h>
#include <BufferQueue.h>

class GeneratorTask : public Task
{
private:
    Generator &_generator;
    BufferQueue &_targetQueue;

public:
    GeneratorTask(Generator &generator, BufferQueue &targetQueue)
        : _generator(generator), _targetQueue(targetQueue)
    {
    }

protected:
    virtual void run()
    {
        size_t bufferSize = 32;
        sample_t buffer[bufferSize];

        while (!stopping())
        {
            _generator.generate(buffer, bufferSize);
            _targetQueue.write(buffer, bufferSize);

            // delay(100);
            // Serial.println("GeneratorTask::run()");
        }
    }
};

#endif // __GENERATORTASK_H__
