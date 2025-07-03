#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int t = -1;
  if(argc > 1)
    t = atoi(argv[1]);
  if(settickets(t) < 0)
    printf("settickets failed\n");
  while(1) {
    asm volatile("":::"memory");
  }
  return 0;
}
