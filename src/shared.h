#ifndef SHARED_H
#define SHARED_H

#include <errno.h>
#include <fcntl.h> // For file operations
#include <linux/futex.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> // For mode
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#define STR_LEN 1024
#define SHARED_MEM_SIZE sizeof(shared_data)
#define SHARED_FILE_PATH "/my_shm_file"

typedef struct {
  char data[STR_LEN];
  int futex;
} shared_data;

void error(const char *msg);
int futex_wait(int *uaddr, int val);
int futex_wake(int *uaddr, int count);
int set_cpu_affinity(int core_id);
int elevate_priority(int priority, int policy);

#endif /* SHARED_H */
