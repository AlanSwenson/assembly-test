#include "random.h"

// Global state for the random number generator
static uint64_t rng_state = 1;

// Initialize the random number generator with a seed
void seed_rng(uint64_t seed) {
    rng_state = seed;
    if (rng_state == 0) {
        rng_state = 1; // Avoid zero state
    }
}

// Generate the next random number using xorshift64
uint64_t next_random(void) {
    rng_state ^= rng_state >> 12;
    rng_state ^= rng_state << 25;
    rng_state ^= rng_state >> 27;
    return rng_state * 0x2545F4914F6CDD1DULL;
}

// Generate a random number in the range [0, max)
// If max is 0, returns 0
uint64_t random_range(uint64_t max) {
    if (max == 0) {
        return 0;
    }
    return next_random() % max;
}

