#include "unity.h"
#include "random.h"
#include <stdio.h>

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Test that RNG can be seeded
void test_rng_seed(void) {
    seed_rng(12345);
    // If we can call this without crashing, the test passes
    TEST_ASSERT_TRUE(1);
}

// Test that RNG produces different values
void test_rng_produces_different_values(void) {
    seed_rng(12345);
    uint64_t val1 = next_random();
    uint64_t val2 = next_random();
    uint64_t val3 = next_random();
    
    // Very unlikely that three consecutive calls produce the same value
    TEST_ASSERT_NOT_EQUAL(val1, val2);
    TEST_ASSERT_NOT_EQUAL(val2, val3);
    TEST_ASSERT_NOT_EQUAL(val1, val3);
}

// Test that different seeds produce different sequences
void test_different_seeds_produce_different_sequences(void) {
    seed_rng(12345);
    uint64_t val1 = next_random();
    
    seed_rng(54321);
    uint64_t val2 = next_random();
    
    // Different seeds should produce different first values
    TEST_ASSERT_NOT_EQUAL(val1, val2);
}

// Test range limiting functionality
void test_random_range(void) {
    seed_rng(12345);
    
    // Test range 0-9 (should produce values 0-9)
    for (int i = 0; i < 100; i++) {
        uint64_t val = random_range(10);
        TEST_ASSERT_TRUE(val >= 0 && val < 10);
    }
    
    // Test range 1-6 (should produce values 1-6)
    for (int i = 0; i < 100; i++) {
        uint64_t val = random_range(6) + 1;
        TEST_ASSERT_TRUE(val >= 1 && val <= 6);
    }
}

// Test that random_range with max=0 doesn't crash
void test_random_range_zero(void) {
    seed_rng(12345);
    uint64_t val = random_range(0);
    TEST_ASSERT_EQUAL(0, val);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_rng_seed);
    RUN_TEST(test_rng_produces_different_values);
    RUN_TEST(test_different_seeds_produce_different_sequences);
    RUN_TEST(test_random_range);
    RUN_TEST(test_random_range_zero);
    
    return UNITY_END();
}

