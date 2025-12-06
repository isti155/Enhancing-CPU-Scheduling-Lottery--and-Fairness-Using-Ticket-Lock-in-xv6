#ifndef TICKETLOCK_H
#define TICKETLOCK_H


#include "types.h"
#include "spinlock.h"

struct ticketlock {
  struct spinlock lk; // protects ticket fields
  uint next;          // next ticket to hand out
  uint turn;          // whose turn it is
};

void ticketlock_init(struct ticketlock *lk);
void ticketlock_acquire(struct ticketlock *lk);
void ticketlock_release(struct ticketlock *lk);

#endif

