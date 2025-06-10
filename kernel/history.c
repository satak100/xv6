#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "riscv.h"
#include "proc.h"
#include "syscall.h"
#include "syscall_stat.h"
#include "defs.h"

#define NSYSCALLS (SYS_history + 1)

struct syscall_info {
  struct spinlock lock;
  char name[16];
  int count;
  int time;
};

static struct syscall_info stats[NSYSCALLS];
static char *names[NSYSCALLS] = {
  [SYS_fork] "fork",
  [SYS_exit] "exit",
  [SYS_wait] "wait",
  [SYS_pipe] "pipe",
  [SYS_read] "read",
  [SYS_kill] "kill",
  [SYS_exec] "exec",
  [SYS_fstat] "fstat",
  [SYS_chdir] "chdir",
  [SYS_dup] "dup",
  [SYS_getpid] "getpid",
  [SYS_sbrk] "sbrk",
  [SYS_sleep] "sleep",
  [SYS_uptime] "uptime",
  [SYS_open] "open",
  [SYS_write] "write",
  [SYS_mknod] "mknod",
  [SYS_unlink] "unlink",
  [SYS_link] "link",
  [SYS_mkdir] "mkdir",
  [SYS_close] "close",
  [SYS_history] "history",
};

void
syscallinit(void)
{
  for(int i = 0; i < NSYSCALLS; i++){
    initlock(&stats[i].lock, "scstat");
    stats[i].count = 0;
    stats[i].time = 0;
    if(names[i])
      safestrcpy(stats[i].name, names[i], sizeof(stats[i].name));
    else
      stats[i].name[0] = '\0';
  }
}

void
syscall_record(int num, int delta)
{
  if(num <= 0 || num >= NSYSCALLS)
    return;
  acquire(&stats[num].lock);
  stats[num].count++;
  stats[num].time += delta;
  release(&stats[num].lock);
}

int
fetch_syscall_stat(int num, struct syscall_stat *st)
{
  if(num <= 0 || num >= NSYSCALLS)
    return -1;
  acquire(&stats[num].lock);
  safestrcpy(st->syscall_name, stats[num].name, sizeof(st->syscall_name));
  st->count = stats[num].count;
  st->accum_time = stats[num].time;
  release(&stats[num].lock);
  return 0;
}

uint64
sys_history(void)
{
  int num;
  uint64 addr;
  struct syscall_stat st;

  argint(0, &num);
  argaddr(1, &addr);
  if(fetch_syscall_stat(num, &st) < 0)
    return -1;
  if(copyout(myproc()->pagetable, addr, (char *)&st, sizeof(st)) < 0)
    return -1;
  return 0;
}

