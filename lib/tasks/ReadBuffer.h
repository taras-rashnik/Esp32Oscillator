#if !defined(__READBUFFER_H__)
#define __READBUFFER_H__

#include <sample.h>
#include <string.h>
#include <noncopyable.hpp>

typedef uint8_t sample_t;

class ReadBuffer : boost::noncopyable
{
private:
    sample_t *const _pArray;
    const size_t _size;
    const size_t _index;

    volatile size_t _readIndex = 0;

public:
    ReadBuffer(sample_t *pArray, size_t size, size_t index)
        : _pArray(pArray), _size(size), _index(index)
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

    void print()
    {
#ifdef PRINT_DEBUG
        Serial.print("ReadBuffer: index(");
        Serial.print(_index);
        Serial.print(")");

        for (size_t i = 0; i < _readIndex; i++)
        {
            Serial.print(" ");
            Serial.print(*(_pArray + i));
        }

        Serial.println();
#endif // PRINT_DEBUG
    }

private:
    size_t freeSpaceToRead()
    {
        return _size - _readIndex;
    }
};

#endif // __READBUFFER_H__
