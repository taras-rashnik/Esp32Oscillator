#if !defined(__READING_TASK_H__)
#define __READING_TASK_H__

#include <Arduino.h>
#include <Task.h>
#include <BufferQueue.h>

class ReadingTask : public Task
{
private:
    BufferQueue &_queue;
    const size_t _chunkSize;
    const size_t _dataLength;
    const uint32_t _delayMs;

    volatile bool _success = true;
    volatile size_t _samplesCounter = 0;

public:
    ReadingTask(BufferQueue &queue, size_t chunkSize, size_t dataLength, uint32_t delayMs)
        : _queue(queue), _chunkSize(chunkSize), _dataLength(dataLength), _delayMs(delayMs)
    {
    }

    bool success() { return _success; }

    size_t bytes() { return _samplesCounter; }

protected:
    virtual void run()
    {
        _samplesCounter = 0;
        sample_t array[_chunkSize];
        size_t numOfIterations = _dataLength / _chunkSize;

        for (size_t i = 0; i < numOfIterations; i++)
        {
            _queue.print();
            _queue.read(array, _chunkSize);

            // check data in buffer
            for (size_t j = 0; j < _chunkSize; j++)
            {
                if (array[j] != _samplesCounter++)
                {
                    _success = false;
                }
            }

            delay(_delayMs);
        }
    }
};

#endif // __READING_TASK_H__
