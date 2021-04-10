// #define PRINT_DEBUG

#include <Arduino.h>
#include <unity.h>
#include <BufferQueue.h>
#include "WritingTask.h"
#include "ReadingTask.h"

void test_constructor(void)
{
    BufferQueue queue(1024, 8);
}

typedef struct
{
    size_t bufferSize;
    size_t numberOfBuffers;
    size_t writeChunkSize;
    size_t readChunkSize;
    size_t dataLength;
    uint32_t writeDelayMs;
    uint32_t readDelayMs;
} TestParams;

void runTest(const TestParams &params)
{
    BufferQueue queue(params.bufferSize, params.numberOfBuffers);
    WritingTask writingTask(queue, params.writeChunkSize, params.dataLength, params.writeDelayMs);
    ReadingTask readingTask(queue, params.readChunkSize, params.dataLength, params.readDelayMs);

    writingTask.start();
    readingTask.start();

    writingTask.join();
    readingTask.join();

    TEST_ASSERT_TRUE(readingTask.success());
    TEST_ASSERT_EQUAL(params.dataLength, readingTask.bytes());
}

void runTestSync(const TestParams &params)
{
    BufferQueue queue(params.bufferSize, params.numberOfBuffers);

    // write all
    sample_t writeArr[params.writeChunkSize];
    size_t writeCounter = 0;
    size_t numWriteIterations = params.dataLength / params.writeChunkSize;

    for (size_t i = 0; i < numWriteIterations; i++)
    {
        for (size_t j = 0; j < params.writeChunkSize; j++)
        {
            writeArr[j] = writeCounter++;
        }

        queue.write(writeArr, params.writeChunkSize);
        queue.print();
    }

    // read all
    sample_t readArr[params.readChunkSize];
    size_t readCounter = 0;
    size_t numReadIterations = params.dataLength / params.readChunkSize;

    for (size_t i = 0; i < numReadIterations; i++)
    {
        queue.read(readArr, params.readChunkSize);
        queue.print();

        for (size_t j = 0; j < params.readChunkSize; j++)
        {
            TEST_ASSERT_EQUAL(readCounter++, readArr[j]);
        }
    }

    TEST_ASSERT_EQUAL(params.dataLength, readCounter);
}

void test_write_read_synchronously(void)
{
    runTestSync({.bufferSize = 4,
                 .numberOfBuffers = 2,
                 .writeChunkSize = 8,
                 .readChunkSize = 8,
                 .dataLength = 8,
                 .writeDelayMs = 20,
                 .readDelayMs = 20});
}

void test_write_read(void)
{
    runTest({.bufferSize = 16,
             .numberOfBuffers = 4,
             .writeChunkSize = 16,
             .readChunkSize = 16,
             .dataLength = 240,
             .writeDelayMs = 20,
             .readDelayMs = 20});
}

void test_write_read2(void)
{
    runTest({.bufferSize = 4,
             .numberOfBuffers = 2,
             .writeChunkSize = 8,
             .readChunkSize = 8,
             .dataLength = 8,
             .writeDelayMs = 20,
             .readDelayMs = 20});
}

void test_write_read3(void)
{
    runTest({.bufferSize = 3,
             .numberOfBuffers = 3,
             .writeChunkSize = 4,
             .readChunkSize = 6,
             .dataLength = 240,
             .writeDelayMs = 20,
             .readDelayMs = 20});
}

void test_write_read4(void)
{
    runTest({.bufferSize = 8,
             .numberOfBuffers = 4,
             .writeChunkSize = 16,
             .readChunkSize = 16,
             .dataLength = 240,
             .writeDelayMs = 20,
             .readDelayMs = 1});
}

void test_write_read5(void)
{
    runTest({.bufferSize = 16,
             .numberOfBuffers = 4,
             .writeChunkSize = 4,
             .readChunkSize = 8,
             .dataLength = 256,
             .writeDelayMs = 1,
             .readDelayMs = 20});
}

void test_write_read6(void)
{
    runTest({.bufferSize = 8,
             .numberOfBuffers = 2,
             .writeChunkSize = 4,
             .readChunkSize = 16,
             .dataLength = 256,
             .writeDelayMs = 0,
             .readDelayMs = 0});
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_constructor);
    RUN_TEST(test_write_read);
    RUN_TEST(test_write_read_synchronously);
    RUN_TEST(test_write_read2);
    RUN_TEST(test_write_read3);
    RUN_TEST(test_write_read4);
    RUN_TEST(test_write_read5);
    RUN_TEST(test_write_read6);
    UNITY_END(); // stop unit testing
}

void loop()
{
    delay(1000);
}