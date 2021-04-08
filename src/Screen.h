#if !defined(__Screen_h__)
#define __Screen_h__

#include <stdint.h>
#include <M5Stack.h>
#include <noncopyable.hpp>
#include "Samples.h"

#define M_SIZE 0.667


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
    Samples _buffer{320};

    void drawGrid()
    {
        const uint16_t cell_size = 40;

        int16_t width = M5.Lcd.width();
        int16_t height = M5.Lcd.height();

        // vertical lines
        uint16_t left = 0;
        while (left < width)
        {
            M5.Lcd.drawFastVLine(left, 0, height, TFT_RED);
            left += cell_size;
        }
        M5.Lcd.drawFastVLine(width-1, 0, height, TFT_RED);

        // horizontal lines
        uint16_t top = 0;
        while (top < height)
        {
            M5.Lcd.drawFastHLine(0, top, width, TFT_RED);
            top += cell_size;
        }
        M5.Lcd.drawFastHLine(0, height-1, width, TFT_RED);
    }

    void drawSamples(Samples& buffer, const Samples &samples, uint32_t color, uint32_t bgcolor)
    {
        int16_t width = M5.Lcd.width();
        int16_t height = M5.Lcd.height();

        int16_t halfHeight = height / 2;

        uint16_t count = min(buffer.size(), samples.size());

        for (uint16_t i = 0; i < count; i++)
        {
            int16_t x = i;
            int16_t y = samples.get(i) + halfHeight;
            int16_t z = buffer.get(i) + halfHeight;

            if (x >= 0 && x < width)
            {
                if (z >= 0 && z < height)
                {
                    // erase old
                    drawPixel(x, z, bgcolor);
                }
                if (y >= 0 && y < height)
                {
                    // draw new
                    drawPixel(x, y, color);
                }
            }

            // copy to buffer
            buffer.set(i, samples.get(i));
        }
    }

public:
    Screen()
    {
    }

    ~Screen()
    {
    }

    void readFrom(const Samples &samples)
    {
        drawSamples(_buffer, samples, TFT_YELLOW, TFT_BLACK);
        drawGrid();
    }
};

#endif // __Screen_h__
