#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct pstat st;
  if(getpinfo(&st) < 0){
    printf("getpinfo failed\n");
    exit(1);
  }
  printf("PID\tQ\torig\tcurr\tticks\n");
  for(int i = 0; i < NPROC; i++){
    if(st.inuse[i]){
      printf("%d\t%d\t%d\t%d\t%d\n", st.pid[i], st.inQ[i], st.tickets_original[i], st.tickets_current[i], st.time_slices[i]);
    }
  }
  exit(0);
}
