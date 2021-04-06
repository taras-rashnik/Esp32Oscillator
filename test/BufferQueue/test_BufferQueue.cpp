#include <Arduino.h>
#include <unity.h>
#include <BufferQueue.h>
#include "WritingTask.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_send_receive_buffer(void) {
    TEST_ASSERT_EQUAL(13, 13);
}

void test_send_receive_empty_buffer(void) {
    TEST_ASSERT_EQUAL(14, 14);
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_send_receive_buffer);
    RUN_TEST(test_send_receive_empty_buffer);
    UNITY_END(); // stop unit testing
}


void loop() {
    delay(1000);
}