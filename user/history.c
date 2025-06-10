#include "kernel/types.h"
#include "kernel/syscall.h"
#include "kernel/syscall_stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct syscall_stat st;
  if(argc == 2){
    int num = atoi(argv[1]);
    if(history(num, &st) < 0){
      fprintf(2, "history: failed\n");
      exit(1);
    }
    printf("%d: syscall: %s, #: %d, time: %d\n", num, st.syscall_name, st.count, st.accum_time);
  } else if(argc == 1){
    for(int i = 1; i <= SYS_history; i++){
      if(history(i, &st) < 0)
        continue;
      printf("%d: syscall: %s, #: %d, time: %d\n", i, st.syscall_name, st.count, st.accum_time);
    }
  } else {
    fprintf(2, "usage: history [syscall_number]\n");
    exit(1);
  }
  exit(0);
}
