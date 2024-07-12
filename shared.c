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
