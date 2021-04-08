#if !defined(__Samples_h__)
#define __Samples_h__

#include <stdint.h>
#include <noncopyable.hpp>

class Samples : boost::noncopyable
{
private:
    uint16_t _size;
    int8_t *_buffer;

public:
    Samples(uint16_t size)
    {
        _size = size;
        _buffer = new int8_t[_size];
    }

    ~Samples()
    {
        delete _buffer;
        _buffer = nullptr;
        _size = 0;
    }

    uint16_t size() const
    {
        return _size;
    }

    void set(uint16_t index, int8_t val)
    {
        if (index < size())
        {
            _buffer[index] = val;
        }
        // else log error
    }

    int8_t get(uint16_t index) const
    {
        if (index < size())
        {
            return _buffer[index];
        }
        // else log error
        return 0;
    }

    void copyTo(Samples &target) const
    {
        uint16_t count = min(size(), target.size());

        for (uint16_t i = 0; i < count; i++)
        {
            target.set(i, get(i));
        }
    }
};

#endif // __Samples_h__
