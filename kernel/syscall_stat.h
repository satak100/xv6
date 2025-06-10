#ifndef SYS_CALL_STAT_H
#define SYS_CALL_STAT_H

struct syscall_stat {
  char syscall_name[16];
  int count;
  int accum_time;
};

#endif // SYS_CALL_STAT_H
