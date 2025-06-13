#include "types.h"
#include "spinlock.h"

// xorshift32 PRNG from https://en.wikipedia.org/wiki/Xorshift
static struct spinlock rand_lock;
static uint32 rand_state = 1;

void
randinit(void)
{
  initlock(&rand_lock, "rand");
  rand_state = 1;
}

void srand(uint32 seed)
{
  acquire(&rand_lock);
  rand_state = seed;
  release(&rand_lock);
}

uint32 rand(void)
{
  acquire(&rand_lock);
  uint32 x = rand_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rand_state = x;
  release(&rand_lock);
  return x;
}

int rand_range(int max)
{
  return rand() % (max + 1);
}
