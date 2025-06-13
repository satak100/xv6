#include "types.h"

// xorshift32 PRNG from https://en.wikipedia.org/wiki/Xorshift
static uint32 rand_state = 1;

void srand(uint32 seed)
{
  rand_state = seed;
}

uint32 rand(void)
{
  uint32 x = rand_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rand_state = x;
  return x;
}

int rand_range(int max)
{
  return rand() % (max + 1);
}
