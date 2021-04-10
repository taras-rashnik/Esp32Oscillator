#include <Arduino.h>
#include <unity.h>
#include <Buffer.h>

const size_t bufferSize = 16;
sample_t bufferArray[bufferSize];

const size_t dataSize = 256;
sample_t sourceArray[dataSize];
sample_t targetArray[dataSize];

void clearBuffer()
{
    memset(bufferArray, 0, sizeof(bufferArray[bufferSize]));
}

void clearTarget()
{
    memset(targetArray, 0, sizeof(targetArray[dataSize]));
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
    clearTarget();
    prepareSource();
}

void tearDown()
{
}

void test_constructor()
{
    Buffer buffer(bufferArray, bufferSize, 0);

    TEST_ASSERT_FALSE(buffer.empty());
    TEST_ASSERT_FALSE(buffer.full());
}

void test_write_read()
{
    Buffer buffer(bufferArray, bufferSize, 0);

    size_t written = buffer.write(sourceArray, dataSize);
    TEST_ASSERT_EQUAL(bufferSize, written);

    size_t read = buffer.read(targetArray, dataSize);
    TEST_ASSERT_EQUAL(bufferSize, read);

    for (size_t i = 0; i < bufferSize; i++)
    {
        TEST_ASSERT_EQUAL(sourceArray[i], targetArray[i]);
    }
    
}

void test_empty_and_full_after_read()
{
    Buffer buffer(bufferArray, bufferSize, 0);

    buffer.write(sourceArray, bufferSize);

    TEST_ASSERT_FALSE(buffer.empty());
    TEST_ASSERT_TRUE(buffer.full());

    buffer.read(targetArray, bufferSize);
    
    TEST_ASSERT_TRUE(buffer.empty());
}

void test_empty_and_full_after_write()
{
    Buffer buffer(bufferArray, bufferSize, 0);

    buffer.write(sourceArray, bufferSize / 2);

    TEST_ASSERT_FALSE(buffer.empty());
    TEST_ASSERT_FALSE(buffer.full());

    buffer.write(sourceArray, bufferSize / 2);

    TEST_ASSERT_FALSE(buffer.empty());
    TEST_ASSERT_TRUE(buffer.full());
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_constructor);
    RUN_TEST(test_write_read);
    RUN_TEST(test_empty_and_full_after_read);
    RUN_TEST(test_empty_and_full_after_write);
    UNITY_END(); // stop unit testing
}

void loop()
{
    delay(1000);
}