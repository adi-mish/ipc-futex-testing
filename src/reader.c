#include "shared.h"

int shm_fd;
shared_data *shared_mem;

static void cleanup_reader(int sig) {
  munmap(shared_mem, SHARED_MEM_SIZE);
  shm_unlink(SHARED_FILE_PATH);
  exit(0);
}

int main() {
  signal(SIGINT, &cleanup_reader);

  shm_fd = shm_open(SHARED_FILE_PATH, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1)
    error("shm_open failed");

  if (ftruncate(shm_fd, SHARED_MEM_SIZE) == -1)
    error("ftruncate failed");

  shared_mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE,
                    MAP_SHARED, shm_fd, 0);
  if (shared_mem == MAP_FAILED)
    error("mmap failed");

  int ret = close(shm_fd);
  if (ret < 0) {
    return ret;
  }
  // Set CPU affinity
  if (set_cpu_affinity(1) == -1) {
    error("set_cpu_affinity");
    return 1;
  }

  // Elevate thread priority
  if (elevate_priority(99) == -1) {
    error("elevate_priority");
    return 1;
  }
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

  return 0;
}
