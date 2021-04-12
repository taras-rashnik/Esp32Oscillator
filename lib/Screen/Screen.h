#if !defined(_SCREEN_H__)
#define _SCREEN_H__

#include <sample.h>
#include <M5Stack.h>
#include <noncopyable.hpp>

// ---------------
// draw pixel
// ---------------
// faster drawPixel method by inlining calls and using setAddrWindow and pushColor
// using macro to force inlining
#define drawPixel(a, b, c)            \
    M5.Lcd.setAddrWindow(a, b, a, b); \
    M5.Lcd.pushColor(c)

class Screen : boost::noncopyable
{
private:
    const size_t Width = M5.Lcd.width();
    const size_t Height = M5.Lcd.height();
    const static size_t SamplesSize = 320;
    sample_t _samples[SamplesSize];

public:
    Screen(/* args */) {}

    void drawSamples(sample_t *pBuffer, size_t bufferSize)
    {
        drawSamples(TFT_BLACK);
        memcpy(_samples, pBuffer, min(bufferSize, SamplesSize));
        drawSamples(TFT_YELLOW);
        drawGrid(TFT_RED);
    }

private:
    void drawSamples(uint32_t color)
    {
        for (size_t i = 0; i < SamplesSize; i++)
        {
            // drawPixel(i, sample(i), color);
            size_t nextIndex = min(Width - 1, i + 1);
            M5.Lcd.drawLine(i, sample(i), nextIndex, sample(nextIndex), color);
        }
    }

    sample_t sample(size_t index)
    {
        return Height - _samples[index];
    }

    void drawGrid(uint32_t color)
    {
        const uint16_t cell_size = 40;

        // vertical lines
        uint16_t left = 0;
        while (left < Width)
        {
            M5.Lcd.drawFastVLine(left, 0, Height, color);
            left += cell_size;
        }
        M5.Lcd.drawFastVLine(Width - 1, 0, Height, color);

        // horizontal lines
        uint16_t top = 0;
        while (top < Height)
        {
            M5.Lcd.drawFastHLine(0, top, Width, color);
            top += cell_size;
        }
        M5.Lcd.drawFastHLine(0, Height - 1, Width, color);
    }
};

#endif // _SCREEN_H__
