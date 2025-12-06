#ifndef _PSTAT_H_
#define _PSTAT_H_

struct pstat {
  int pid;
  int base_tickets;
  int acc_tickets;
  int exch_tickets;
  int dependency;
  int total_ticks;
};

#endif
