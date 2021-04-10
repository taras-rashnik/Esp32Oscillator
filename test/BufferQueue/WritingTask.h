#if !defined(__WRITING_TASK_H__)
#define __WRITING_TASK_H__

#include <Arduino.h>
#include <Task.h>
#include <BufferQueue.h>

class WritingTask : public Task
{
private:
    BufferQueue &_queue;
    const size_t _chunkSize;
    const size_t _dataLength;
    const uint32_t _delayMs;

public:
    WritingTask(BufferQueue &queue, size_t chunkSize, size_t dataLength, uint32_t delayMs)
        : _queue(queue), _chunkSize(chunkSize), _dataLength(dataLength), _delayMs(delayMs)
    {
    }

protected:
    virtual void run()
    {
        size_t samplesCounter = 0;
        sample_t array[_chunkSize];
        size_t numOfIterations = _dataLength / _chunkSize;

        for (size_t i = 0; i < numOfIterations; i++)
        {
            // prepare buffer
            for (size_t j = 0; j < _chunkSize; j++)
            {
                array[j] = samplesCounter++;
            }

            _queue.write(array, _chunkSize);
            _queue.print();
            delay(_delayMs);
        }
    }
};

#endif // __WRITING_TASK_H__
