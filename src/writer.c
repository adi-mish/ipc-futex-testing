#include "shared.h"
#include <stdio.h>

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

  shm_fd = shm_open(SHARED_FILE_PATH, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1)
    error("shm_open failed");

  if (ftruncate(shm_fd, sizeof(shared_data)) == -1)
    error("ftruncate failed");

  shared_mem = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE,
                    MAP_SHARED, shm_fd, 0);
  if (shared_mem == MAP_FAILED)
    error("mmap failed");

  // Initialize the futex and shared data
  shared_mem->futex = 0;

  while (1) {
    // Write to shared memory
    time_t now = time(NULL);
    snprintf(shared_mem->data, sizeof(shared_mem->data), "Current time: %s",
             ctime(&now));

    // Set futex to 1 atomically
    shared_mem->futex = 1;

    // Wake up the reader
    int ret = futex_wake(&shared_mem->futex, 1);
    if (ret == -1)
      error("futex_wake failed");

    sleep(1); // Optional delay between writes
  }

  return 0;
}
