#include "unity.h"
#include <stdio.h>

// Test setup and teardown
void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Sample test to verify Unity is working
void test_unity_works(void) {
    TEST_ASSERT_EQUAL(1, 1);
    TEST_ASSERT_TRUE(1 == 1);
    TEST_ASSERT_FALSE(0 == 1);
}

// Test runner
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_unity_works);
    
    return UNITY_END();
}

