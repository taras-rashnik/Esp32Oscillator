#if !defined(__BUFFER_QUEUE_H__)
#define __BUFFER_QUEUE_H__

#include <stdint.h>
#include <stddef.h>
#include <noncopyable.hpp>

typedef uint8_t sample_t;

class Buffer : boost::noncopyable
{
private:
    sample_t* const _pArray;
    const size_t _size;

public:
    Buffer(sample_t *pArray, size_t size)
        : _pArray(pArray), _size(size)
    {
    }

    void clear()
    {
        memset(_pArray, 0, sizeof(sample_t) * _size);
    }
};

class BufferQueue : boost::noncopyable
{
private:
    const size_t _bufferSize;
    const size_t _numberOfBuffers;

    const QueueHandle_t _fullQueueHandle;
    const QueueHandle_t _emptyQueueHandle;

    sample_t *_pArray = NULL;

public:
    BufferQueue(size_t bufferSize, size_t numberOfBuffers)
        : _bufferSize(bufferSize),
          _numberOfBuffers(numberOfBuffers),
          _fullQueueHandle(xQueueCreate(numberOfBuffers, sizeof(Buffer *))),
          _emptyQueueHandle(xQueueCreate(numberOfBuffers, sizeof(Buffer *)))
    {
        _pArray = new sample_t[bufferSize * numberOfBuffers];

        for (size_t i = 0; i < numberOfBuffers; i++)
        {
            Buffer *pBuffer = new Buffer(_pArray + i, bufferSize);
            pBuffer->clear();
            xQueueSend(_emptyQueueHandle, pBuffer, portMAX_DELAY);
        }
    }

    ~BufferQueue()
    {
        // TODO: delete all Buffers* inside both queue
        vQueueDelete(_fullQueueHandle);
        vQueueDelete(_emptyQueueHandle);
        delete _pArray;
        _pArray = NULL;
    }

    void write(const sample_t *pSourceArray, size_t size)
    {
    }

    void read(sample_t *pTargetArray, size_t size)
    {
    }
};

#endif // __BUFFER_QUEUE_H__
