#if !defined(__BUFFER_H__)
#define __BUFFER_H__

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <noncopyable.hpp>

typedef uint8_t sample_t;

class WriteBuffer : boost::noncopyable
{
private:
    sample_t *const _pArray;
    const size_t _size;

    volatile size_t _writeIndex = 0;

public:
    WriteBuffer(sample_t *pArray, size_t size)
        : _pArray(pArray), _size(size)
    {
        clear();
    }

    // only for write operations
    bool full() { return freeSpaceToWrite() <= 0; }

    size_t write(const sample_t *pSourceArray, size_t size)
    {
        size_t minSize = min(size, freeSpaceToWrite());
        memcpy(_pArray + _writeIndex, pSourceArray, minSize);
        _writeIndex += minSize;
        return minSize;
    }

    void clear()
    {
        memset(_pArray, 0, sizeof(sample_t[_size]));
        _writeIndex = 0;
    }

private:
    size_t freeSpaceToWrite()
    {
        return _size - _writeIndex;
    }
};

class ReadBuffer : boost::noncopyable
{
private:
    sample_t *const _pArray;
    const size_t _size;

    volatile size_t _readIndex = 0;

public:
    ReadBuffer(sample_t *pArray, size_t size)
        : _pArray(pArray), _size(size)
    {
        clear();
    }

    // only for read operations
    bool empty() { return freeSpaceToRead() <= 0; }

    size_t read(sample_t *pTargetArray, size_t size)
    {
        size_t minSize = min(size, freeSpaceToRead());
        memcpy(pTargetArray, _pArray + _readIndex, minSize);
        _readIndex += minSize;
        return minSize;
    }

    void clear()
    {
        _readIndex = 0;
    }

private:
    size_t freeSpaceToRead()
    {
        return _size - _readIndex;
    }
};

class Buffer : public WriteBuffer, public ReadBuffer
{
public:
    Buffer(sample_t *pArray, size_t size)
        : WriteBuffer(pArray, size), ReadBuffer(pArray, size)
    {
    }

    // void clear()
    // {
    //     WriteBuffer::clear();
    //     ReadBuffer::clear();
    // }
};

#endif // __BUFFER_H__
