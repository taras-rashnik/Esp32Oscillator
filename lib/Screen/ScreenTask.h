#if !defined(__SCREENTASK_H__)
#define __SCREENTASK_H__

#include <sample.h>
#include <Task.h>
#include <Screen.h>
#include <BufferQueue.h>

class ScreenTask : public Task
{
private:
    Screen &_screen;
    BufferQueue &_sourceQueue;

public:
    ScreenTask(Screen &screen, BufferQueue &sourceQueue)
        : _screen(screen), _sourceQueue(sourceQueue)
    {
    }

protected:
    virtual void run()
    {
        size_t bufferSize = 320;
        sample_t buffer[bufferSize];

        while (!stopping())
        {
            _sourceQueue.read(buffer, bufferSize);
            _screen.drawSamples(buffer, bufferSize);

            delay(40);
            // Serial.println("ScreenTask::run()");
        }
    }
};

#endif // __SCREENTASK_H__
