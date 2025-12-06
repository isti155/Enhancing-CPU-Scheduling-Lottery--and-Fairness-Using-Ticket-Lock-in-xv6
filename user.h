#ifndef USER_H
#define USER_H

struct stat;
struct rtcdate;

#include "pstat.h"   // allowed (struct only), safe to include

// System calls (user-level stubs generated in usys.S)
int fork(void);
void exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);

// Your added system calls
int set_base_tickets(int n);
int update_ticket_status(struct pstat *st);
int grant_reward(int n);

// ulib functions
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

#endif
