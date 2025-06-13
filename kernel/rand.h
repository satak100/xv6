#ifndef _RAND_H_
#define _RAND_H_
#include "types.h"
void srand(uint32 seed);
uint32 rand(void);
int rand_range(int max);
#endif
