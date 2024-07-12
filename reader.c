#include "shared.h"

int shm_fd;
shared_data *shared_mem;

void cleanup(int sig) {
  munmap(shared_mem, sizeof(shared_data));
  close(shm_fd);
  shm_unlink(SHARED_FILE_PATH);
  exit(0);
}

int main() {
  signal(SIGINT, cleanup);

  shm_fd = shm_open(SHARED_FILE_PATH, O_RDWR, 0666);
  if (shm_fd == -1)
    error("shm_open failed");

  shared_mem = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE,
                    MAP_SHARED, shm_fd, 0);
  if (shared_mem == MAP_FAILED)
    error("mmap failed");

  while (1) {
    // Wait until the futex is set to 1 by the writer
    while (1) {
      int futex_val = __sync_val_compare_and_swap(&shared_mem->futex, 1, 0);
      if (futex_val == 1) {
        break;
      }
      // Wait for the futex to become 1
      if (futex_wait(&shared_mem->futex, 0) == -1) {
        if (errno != EAGAIN) {
          error("futex_wait failed");
        }
      }
    }

    // Read from shared memory
    printf("Message received: %s\n", shared_mem->data);
  }

  // Clean up
  munmap(shared_mem, sizeof(shared_data));
  close(shm_fd);
  shm_unlink(SHARED_FILE_PATH);
  return 0;
}
