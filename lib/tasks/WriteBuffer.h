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
    const size_t _index;

    volatile size_t _writeIndex = 0;

public:
    WriteBuffer(sample_t *pArray, size_t size, size_t index)
        : _pArray(pArray), _size(size), _index(index)
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

    void print()
    {
#ifdef PRINT_DEBUG
        Serial.print("WriteBuffer: index(");
        Serial.print(_index);
        Serial.print(")");

        for (size_t i = 0; i < _writeIndex; i++)
        {
            Serial.print(" ");
            Serial.print(*(_pArray + i));
        }

        Serial.println();
#endif // PRINT_DEBUG
    }

private:
    size_t freeSpaceToWrite()
    {
        return _size - _writeIndex;
    }
};

#endif // __WRITEBUFFER_H__
