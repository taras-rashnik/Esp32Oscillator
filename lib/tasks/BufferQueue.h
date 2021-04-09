#if !defined(__BUFFER_QUEUE_H__)
#define __BUFFER_QUEUE_H__

#include <stdint.h>
#include <stddef.h>
#include <noncopyable.hpp>
#include "Buffer.h"

class BufferQueue : boost::noncopyable
{
private:
    const size_t _bufferSize;
    const size_t _numberOfBuffers;

    const QueueHandle_t _fullQueueHandle;
    const QueueHandle_t _emptyQueueHandle;

    sample_t *_pArray = NULL;

    WriteBuffer *_currentWriteBuffer = NULL;
    ReadBuffer *_currentReadBuffer = NULL;

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
            // fill emptyQueue with new empty Buffers
            Buffer *pBuffer = new Buffer(_pArray + i, _bufferSize);
            xQueueSend(_emptyQueueHandle, &pBuffer, portMAX_DELAY);
        }
    }

    ~BufferQueue()
    {
        // TODO: free all Buffers in both queues
        vQueueDelete(_fullQueueHandle);
        vQueueDelete(_emptyQueueHandle);
        delete _pArray;
        _pArray = NULL;
    }

    // emptyQueue -> write() -> fullQueue
    void write(const sample_t *pSourceArray, size_t size)
    {
        do
        {
            WriteBuffer *pBuffer = writeBuffer();
            size_t written = pBuffer->write(pSourceArray, size);

            size -= written;
            pSourceArray += written;

        } while (size > 0);

        sendWriteBufferIfFull();
    }

    // fullQueue -> read() -> emptyQueue
    void read(sample_t *pTargetArray, size_t size)
    {
        do
        {
            ReadBuffer *pBuffer = readBuffer();
            size_t read = pBuffer->read(pTargetArray, size);

            size -= read;
            pTargetArray += read;

        } while (size > 0);

        sendBackReadBufferIfEmpty();
    }

private:
    WriteBuffer *writeBuffer()
    {
        if (_currentWriteBuffer == NULL)
        {
            receiveNewEmptyWriteBuffer();
        }
        else if (_currentWriteBuffer->full())
        {
            sendWriteBufferIfFull();
            receiveNewEmptyWriteBuffer();
        }

        return _currentWriteBuffer;
    }

    void sendWriteBufferIfFull()
    {
        if (_currentWriteBuffer != NULL && _currentWriteBuffer->full())
        {
            xQueueSend(_fullQueueHandle, &_currentWriteBuffer, portMAX_DELAY);
            _currentWriteBuffer = NULL;
        }
    }

    void receiveNewEmptyWriteBuffer()
    {
        if (_currentWriteBuffer == NULL)
        {
            xQueueReceive(_emptyQueueHandle, &_currentWriteBuffer, portMAX_DELAY);
            _currentWriteBuffer->clear();
        }
    }

    ReadBuffer *readBuffer()
    {
        if (_currentReadBuffer == NULL)
        {
            receiveNewFullReadBuffer();
        }
        else if (_currentReadBuffer->empty())
        {
            sendBackReadBufferIfEmpty();
            receiveNewFullReadBuffer();
        }

        return _currentReadBuffer;
    }

    void receiveNewFullReadBuffer()
    {
        if (_currentReadBuffer == NULL)
        {
            xQueueReceive(_fullQueueHandle, &_currentReadBuffer, portMAX_DELAY);
        }
    }

    void sendBackReadBufferIfEmpty()
    {
        if (_currentReadBuffer != NULL && _currentReadBuffer->empty())
        {
            _currentReadBuffer->clear();
            xQueueSend(_emptyQueueHandle, &_currentReadBuffer, portMAX_DELAY);
            _currentReadBuffer = NULL;
        }
    }
};

#endif // __BUFFER_QUEUE_H__
