#if !defined(__GENERATOR_H__)
#define __GENERATOR_H__

#include <sample.h>
#include <noncopyable.hpp>

enum class Waveform
{
    Sine,
    Triangle,
    Square,
    Sawtooth
};

struct GeneratorParameters
{
    size_t samplingFrequency;
    size_t signalFrequency;
    Waveform waveform;
    sample_t amplitude;
};

class Generator : boost::noncopyable
{
private:
    GeneratorParameters _parameters;
    size_t _index = 0;
    sample_t _value = 0;

public:
    Generator(GeneratorParameters parameters) : _parameters(parameters)
    {
    }

    void generate(sample_t *pBuffer, size_t bufferSize)
    {
        for (size_t i = 0; i < bufferSize; i++)
        {
            *(pBuffer + i) = nextSample();
        }
    }

private:
    sample_t nextSample()
    {
        sample_t start = (256 - _parameters.amplitude) / 2;
        sample_t end = 256 - start;
        size_t delta = _parameters.amplitude * _parameters.signalFrequency / _parameters.samplingFrequency;

        _value += delta;

        if (_value > end)
        {
            _value = start;
        }

        return _value;
    }
};

#endif // __GENERATOR_H__
