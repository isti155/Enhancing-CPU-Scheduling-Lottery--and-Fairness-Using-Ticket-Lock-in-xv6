#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "ticketlock.h"

void
ticketlock_init(struct ticketlock *lk)
{
  initlock(&lk->lk, "ticketlk");
  lk->next = 0;
  lk->turn = 0;
}

void
ticketlock_acquire(struct ticketlock *lk)
{
  // atomically get a ticket
  acquire(&lk->lk);
  uint my = lk->next;
  lk->next++;
  release(&lk->lk);

  // wait for our turn (busy wait)
  while(1){
    acquire(&lk->lk);
    if(lk->turn == my){
      release(&lk->lk);
      break;
    }
    release(&lk->lk);
    // yield CPU so other procs can run (optional)
    yield(); // safe to call if present; else simple busy loop
  }
}

void
ticketlock_release(struct ticketlock *lk)
{
  acquire(&lk->lk);
  lk->turn++;
  release(&lk->lk);
}
