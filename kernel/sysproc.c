#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_settickets(void)
{
  int n;
  argint(0, &n);
  if(n < 1)
    n = DEFAULT_TICKET_COUNT;
  struct proc *p = myproc();
  acquire(&p->lock);
  p->ticket_original = n;
  p->ticket_current = n;
  release(&p->lock);
  return 0;
}

uint64
sys_getpinfo(void)
{
  uint64 addr;
  struct pstat st;
  argaddr(0, &addr);
  for(int i = 0; i < NPROC; i++){
    st.pid[i] = 0;
    st.inuse[i] = 0;
    st.inQ[i] = 0;
    st.tickets_original[i] = 0;
    st.tickets_current[i] = 0;
    st.time_slices[i] = 0;
  }
  struct proc *pp;
  for(pp = proc; pp < &proc[NPROC]; pp++){
    acquire(&pp->lock);
    if(pp->state != UNUSED){
      int idx = pp - proc;
      st.pid[idx] = pp->pid;
      st.inuse[idx] = 1;
      st.inQ[idx] = pp->queue;
      st.tickets_original[idx] = pp->ticket_original;
      st.tickets_current[idx] = pp->ticket_current;
      st.time_slices[idx] = pp->ticks_total;
    }
    release(&pp->lock);
  }
  if(copyout(myproc()->pagetable, addr, (char *)&st, sizeof(st)) < 0)
    return -1;
  return 0;
}
