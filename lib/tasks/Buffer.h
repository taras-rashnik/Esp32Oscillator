#if !defined(__BUFFER_H__)
#define __BUFFER_H__

#include "WriteBuffer.h"
#include "ReadBuffer.h"

class Buffer : public WriteBuffer, public ReadBuffer
{
public:
    Buffer(sample_t *pArray, size_t size)
        : WriteBuffer(pArray, size), ReadBuffer(pArray, size)
    {
    }
};

#endif // __BUFFER_H__
