#define UNITY_INCLUDE_EXEC_TIME

#include <Arduino.h>
#include <unity.h>
#include "TestTask.h"

void test_join(void)
{
    TestTask task;

    TEST_ASSERT_FALSE(task.running());

    task.start();
    delay(10);
    TEST_ASSERT_TRUE(task.running());

    task.join();
    TEST_ASSERT_FALSE(task.running());
}

void test_stop(void)
{
    TestTask task;

    TEST_ASSERT_FALSE(task.running());

    task.start();
    delay(10);
    TEST_ASSERT_TRUE(task.running());

    task.stop();
    TEST_ASSERT_FALSE(task.running());
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_join);
    RUN_TEST(test_stop);
    UNITY_END(); // stop unit testing
}

void loop()
{
    delay(1000);
}