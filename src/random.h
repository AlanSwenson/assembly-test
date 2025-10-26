#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

// Initialize the random number generator with a seed
void seed_rng(uint64_t seed);

// Generate the next random number
uint64_t next_random(void);

// Generate a random number in the range [0, max)
// If max is 0, returns 0
uint64_t random_range(uint64_t max);

#endif // RANDOM_H

