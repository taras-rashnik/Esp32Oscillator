#include <Arduino.h>
#include <unity.h>
#include <ReadBuffer.h>

const size_t bufferSize = 16;
sample_t bufferArray[bufferSize];

const size_t dataSize = 256;
sample_t targetArray[dataSize];

void clearBuffer()
{
    for (size_t i = 0; i < bufferSize; i++)
    {
        bufferArray[i] = i;
    }
}

void clearTarget()
{
    memset(targetArray, 0, sizeof(targetArray[dataSize]));
}

void setUp()
{
    clearBuffer();
    clearTarget();
}

void tearDown()
{
}

void test_constructor()
{
    ReadBuffer buffer(bufferArray, bufferSize, 0);
    TEST_ASSERT_FALSE(buffer.empty());
}

void test_empty_after_read()
{
    ReadBuffer buffer(bufferArray, bufferSize, 0);

    buffer.read(targetArray, bufferSize);
    TEST_ASSERT_TRUE(buffer.empty());
}

void test_empty_after_clear()
{
    ReadBuffer buffer(bufferArray, bufferSize, 0);

    buffer.read(targetArray, bufferSize);
    TEST_ASSERT_TRUE(buffer.empty());

    buffer.clear();
    TEST_ASSERT_FALSE(buffer.empty());
}

void test_read()
{
    ReadBuffer buffer(bufferArray, bufferSize, 0);

    size_t read = buffer.read(targetArray, dataSize);
    TEST_ASSERT_EQUAL_INT32(bufferSize, read);

    for (size_t i = 0; i < bufferSize; i++)
    {
        TEST_ASSERT_EQUAL(i, targetArray[i]);
    }
}

void test_read2()
{
    ReadBuffer buffer(bufferArray, bufferSize, 0);

    size_t read = buffer.read(targetArray, bufferSize / 2);
    TEST_ASSERT_EQUAL_INT32(bufferSize / 2, read);

    read = buffer.read(targetArray + read, bufferSize / 2);
    TEST_ASSERT_EQUAL_INT32(bufferSize / 2, read);

    for (size_t i = 0; i < bufferSize; i++)
    {
        TEST_ASSERT_EQUAL(i, targetArray[i]);
    }
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_constructor);
    RUN_TEST(test_empty_after_read);
    RUN_TEST(test_empty_after_clear);
    RUN_TEST(test_read);
    RUN_TEST(test_read2);
    UNITY_END();
}

void loop()
{
    delay(1000);
}