#if !defined(__WRITEBUFFER_H__)
#define __WRITEBUFFER_H__

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

#endif // __WRITEBUFFER_H__
