// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
};

/* --- add to the end of spinlock.h --- */

/*
 * Ticket lock for FIFO / fairness.
 * next_ticket: the next available ticket to hand out (fetch-and-inc)
 * now_serving: the ticket currently being served
 */
struct ticketlock {
  uint next_ticket;   // next ticket number to give
  uint now_serving;   // current ticket being served
  char *name;
  struct cpu *cpu;    // cpu holding the lock (for debugging)
  uint pcs[10];
};

void init_ticketlock(struct ticketlock *lk, char *name);
void acquire_ticketlock(struct ticketlock *lk);
void release_ticketlock(struct ticketlock *lk);
int  holding_ticketlock(struct ticketlock *lk);
