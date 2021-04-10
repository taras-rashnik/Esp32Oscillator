#if !defined(__BUFFER_H__)
#define __BUFFER_H__

#include "WriteBuffer.h"
#include "ReadBuffer.h"

class Buffer : public WriteBuffer, public ReadBuffer
{
public:
    Buffer(sample_t *pArray, size_t size, size_t index)
        : WriteBuffer(pArray, size, index), ReadBuffer(pArray, size, index)
    {
    }
};

#endif // __BUFFER_H__
