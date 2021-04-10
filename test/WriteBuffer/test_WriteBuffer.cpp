#include <Arduino.h>
#include <unity.h>
#include <WriteBuffer.h>

const size_t bufferSize = 16;
sample_t bufferArray[bufferSize];

const size_t dataSize = 256;
sample_t sourceArray[dataSize];

void clearBuffer()
{
    memset(bufferArray, 0, sizeof(bufferArray[bufferSize]));
}

void prepareSource()
{
    for (size_t i = 0; i < dataSize; i++)
    {
        sourceArray[i] = i;
    }
}

void setUp()
{
    clearBuffer();
    prepareSource();
}

void tearDown()
{
}

void test_constructor()
{
    WriteBuffer buffer(bufferArray, bufferSize, 0);
    TEST_ASSERT_FALSE(buffer.full());
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_constructor);
    UNITY_END();
}

void loop()
{
    delay(1000);
}
