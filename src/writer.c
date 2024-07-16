#include "shared.h"
#include <glib.h>

int shm_fd;
shared_data *shared_mem;
GMainLoop *main_loop;

static void cleanup_writer(int sig) {
  munmap(shared_mem, SHARED_MEM_SIZE);
  shm_unlink(SHARED_FILE_PATH);
  if (main_loop) {
    g_main_loop_quit(main_loop);
  }
  exit(0);
}

gboolean write_to_shared_memory(gpointer user_data) {
  static int counter = 0;
  char message[256];

  // Get current time in milliseconds
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  long milliseconds =
      ts.tv_nsec / 1000000; // Convert nanoseconds to milliseconds

  // Write message to shared memory
  snprintf(message, sizeof(message), "Message %d - Timestamp: %ld ms\n",
           counter, milliseconds);
  strcpy(shared_mem->data, message);

  // Set futex to 1 atomically
  shared_mem->futex = 1;

  // Wake up the reader
  int ret = futex_wake(&shared_mem->futex, 1);
  if (ret == -1) {
    error("futex_wake failed");
  }

  counter++; // Increment message counter

  return TRUE; // Continue calling this function
}

int main() {
  signal(SIGINT, &cleanup_writer);

  shm_fd = shm_open(SHARED_FILE_PATH, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1)
    error("shm_open failed");

  if (ftruncate(shm_fd, SHARED_MEM_SIZE) == -1)
    error("ftruncate failed");

  shared_mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                    shm_fd, 0);
  if (shared_mem == MAP_FAILED)
    error("mmap failed");

  int ret = close(shm_fd);
  if (ret < 0) {
    return ret;
  }

  // Initialize the futex and shared data
  shared_mem->futex = 0;

  // Set CPU affinity
  if (set_cpu_affinity(0) == -1) {
    error("set_cpu_affinity");
    return 1;
  }

  // Elevate thread priority
  if (elevate_priority(99, SCHED_FIFO) == -1) {
    error("elevate_priority");
    return 1;
  }

  // Initialize the GMainLoop
  main_loop = g_main_loop_new(NULL, FALSE);
  
  // Set up the timer to write to shared memory every 10 milliseconds
  g_timeout_add(10, write_to_shared_memory, NULL);

  // Run the main loop
  g_main_loop_run(main_loop);

  // Run the main loop
  g_main_loop_run(main_loop);

  // Cleanup after the main loop exits
  g_main_loop_unref(main_loop);
  cleanup_writer(0); // Call cleanup to unmap shared memory and unlink it

  return 0;
}