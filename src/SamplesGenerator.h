#if !defined(__SamplesGenerator_h__)
#define __SamplesGenerator_h__

#include <stdint.h>
#include "boost/noncopyable.hpp"
#include "Samples.h"

class SamplesGenerator : boost::noncopyable
{
private:
    int8_t _val = 0;
    int8_t _start = 0;


public:
    SamplesGenerator()
    {
    }

    ~SamplesGenerator()
    {
    }

    void writeTo(Samples& samples)
    {
        _val = _start;
        _start += 5;
        if(_start > 120){
            _start = -120;
        }
        
        for (uint16_t i = 0; i < samples.size(); i++)
        {
            samples.set(i, _val++);
            if (_val > 120)
            {
                _val = -120;
            }
        }
    }
};

#endif // __SamplesGenerator_h__
