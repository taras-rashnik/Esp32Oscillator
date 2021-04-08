#include <Arduino.h>
#include <unity.h>
#include <BufferQueue.h>
#include "WritingTask.h"
#include "ReadingTask.h"

void test_constructor(void)
{
    BufferQueue queue(1024, 8);
}

void test_write_read(void)
{
    size_t bufferSize = 16;
    size_t numberOfBuffers = 4;
    size_t writeChunkSize = 16;
    size_t readChunkSize = 16;
    size_t dataLength = 240;
    uint32_t writeDelayMs = 20;
    uint32_t readDelayMs = 20;

    BufferQueue queue(bufferSize, numberOfBuffers);
    WritingTask writingTask(queue, writeChunkSize, dataLength, writeDelayMs);
    ReadingTask readingTask(queue, readChunkSize, dataLength, readDelayMs);

    writingTask.start();
    readingTask.start();

    writingTask.join();
    readingTask.join();

    TEST_ASSERT_TRUE(readingTask.success());
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_constructor);
    RUN_TEST(test_write_read);
    UNITY_END(); // stop unit testing
}

void loop()
{
    delay(1000);
}