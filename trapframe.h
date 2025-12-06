#ifndef _TRAPFRAME_H_
#define _TRAPFRAME_H_

#include "types.h"

struct trapframe {
  // ↓↓↓ COPY EXACT trapframe FIELDS FROM YOUR trap.c ↓↓↓
  uint edi;
  uint esi;
  uint ebp;
  uint oesp;      // useless & ignored
  uint ebx;
  uint edx;
  uint ecx;
  uint eax;

  ushort gs, padding1;
  ushort fs, padding2;
  ushort es, padding3;
  ushort ds, padding4;

  uint trapno;

  // below here is hardware-defined
  uint err;
  uint eip;
  uint cs;
  uint eflags;

  // only when crossing rings, hardware pushes these:
  uint esp;
  uint ss;
};

#endif
