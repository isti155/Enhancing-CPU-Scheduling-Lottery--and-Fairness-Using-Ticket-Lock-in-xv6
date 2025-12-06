#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pstat.h"
#include "spinlock.h"   // <-- add this line

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_set_base_tickets(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;

  if(n < 0)
    return -1;

  struct proc *p = myproc();

  acquire(&ptable.lock);
  p->base_tickets = n;
  release(&ptable.lock);

  return 0;
}


int sys_update_ticket_status(void)
{
  struct pstat *ustat;

  if(argptr(0, (void*)&ustat, sizeof(*ustat)) < 0)
    return -1;

  struct proc *p = myproc();
  struct pstat kstat;

  acquire(&ptable.lock);
  kstat.pid          = p->pid;
  kstat.base_tickets = p->base_tickets;
  kstat.acc_tickets  = p->acc_tickets;
  kstat.exch_tickets = p->exch_tickets;
  kstat.dependency   = p->dependency;
  kstat.total_ticks  = p->total_ticks;
  release(&ptable.lock);

  if(copyout(p->pgdir, (uint)ustat, (void*)&kstat, sizeof(kstat)) < 0)
    return -1;

  return 0;
}

int
sys_grant_reward(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  if(n <= 0)
    return -1;

  struct proc *p = myproc();
  if(!p) return -1;

  acquire(&ptable.lock);
  p->reward_tickets += n;
  release(&ptable.lock);
  return 0;
}

