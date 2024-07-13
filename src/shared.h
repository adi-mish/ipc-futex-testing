#ifndef SHARED_H
#define SHARED_H

#include <errno.h>
#include <fcntl.h> // For file operations
#include <linux/futex.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> // For mode
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#define SHARED_MEM_SIZE 1024
#define SHARED_FILE_PATH "/my_shm_file"

typedef struct {
  char data[SHARED_MEM_SIZE];
  int futex;
} shared_data;

void error(const char *msg);
int futex_wait(int *uaddr, int val);
int futex_wake(int *uaddr, int count);

#endif /* SHARED_H */
