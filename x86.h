#ifndef X86_H
#define X86_H

#include "types.h"   // For uint, uchar, ushort

// ---------------------
//  Basic Port I/O
// ---------------------
static inline uchar
inb(ushort port)
{
  uchar data;
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  return data;
}

static inline void
insl(int port, void *addr, int cnt)
{
  asm volatile("cld; rep insl" :
               "=D" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "memory", "cc");
}

static inline void
outb(ushort port, uchar data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void
outsl(int port, const void *addr, int cnt)
{
  asm volatile("cld; rep outsl" :
               "=S" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "cc");
}

// ---------------------
//  Memory Fill Helpers
// ---------------------
static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

static inline void
stosl(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosl" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

// ---------------------
//  EFLAGS, Interrupts
// ---------------------
static inline uint
readeflags(void)
{
  uint eflags;
  asm volatile("pushfl; popl %0" : "=r" (eflags));
  return eflags;
}

static inline void cli(void) { asm volatile("cli"); }
static inline void sti(void) { asm volatile("sti"); }

// ---------------------
//  Atomic Exchange
// ---------------------
static inline uint
xchg(volatile uint *addr, uint newval)
{
  uint result;
  asm volatile("lock; xchgl %0,%1"
               : "+m" (*addr), "=a" (result)
               : "1" (newval)
               : "cc");
  return result;
}

// ---------------------
//  Control Registers
// ---------------------
static inline uint
rcr2(void)
{
  uint val;
  asm volatile("movl %%cr2,%0" : "=r" (val));
  return val;
}

static inline void
lcr3(uint val)
{
  asm volatile("movl %0,%%cr3" : : "r" (val));
}

// ---------------------
//  Descriptor Table Loaders
// ---------------------
static inline void
ltr(ushort sel)
{
  asm volatile("ltr %0" : : "r" (sel));
}

static inline void
lgdt(void *p, int size)
{
  // Load the GDT register: limit (size-1) and base pointer
  ushort pd[3];
  pd[0] = size - 1;
  pd[1] = (uint)p & 0xFFFF;
  pd[2] = ((uint)p) >> 16;
  asm volatile("lgdt (%0)" : : "r" (pd));
}

static inline void
lidt(void *p, int size)
{
  ushort pd[3];
  pd[0] = size - 1;
  pd[1] = (uint)p & 0xFFFF;
  pd[2] = ((uint)p) >> 16;
  asm volatile("lidt (%0)" : : "r" (pd));
}

// ---------------------
//   SEG16 Macro (required by vm.c)
// ---------------------
// For TSS descriptors.
#define SEG16(type, base, lim, dpl) (struct segdesc){      \
  (uint)(lim) & 0xFFFF,                                     \
  (uint)(base) & 0xFFFF,                                    \
  ((uint)(base) >> 16) & 0xFF,                              \
  (type),                                                   \
  0,                                                        \
  (dpl),                                                    \
  1,                                                        \
  ((uint)(lim) >> 16) & 0xF,                                \
  0, 0, 0, 0,                                               \
  ((uint)(base) >> 24) & 0xFF                               \
}

#endif // X86_H
