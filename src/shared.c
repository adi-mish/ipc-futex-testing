#define _GNU_SOURCE
#include "shared.h"

void error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int futex_wait(int *uaddr, int val) {
  return syscall(SYS_futex, uaddr, FUTEX_WAIT, val, NULL, NULL, 0);
}

int futex_wake(int *uaddr, int count) {
  return syscall(SYS_futex, uaddr, FUTEX_WAKE, count, NULL, NULL, 0);
}

int set_cpu_affinity(int core_id) {
  cpu_set_t cpu;
  CPU_ZERO(&cpu);
  CPU_SET(core_id, &cpu);
  fprintf(stderr, "Setting CPU Affinity to CPU %d...\n", core_id);
  int ret = sched_setaffinity(0, sizeof(cpu_set_t), &cpu);
  return ret;
}

int elevate_priority(int priority) {
  struct sched_param params;
  params.sched_priority = priority;
  fprintf(stderr, "Setting thread priority to %d...\n", priority);
  int ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &params);
  return ret;
}