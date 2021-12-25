#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
unsigned int sum = 0;
volatile int pending_reads;

void interrupt_service_routine () {
  void* val;
  void (*callback)(void*,void*);
  queue_dequeue (pending_read_queue, &val, NULL, &callback);
  callback (val, NULL);
}

void handle_read (void* resultv, void* not_used) {
  unsigned int* result = (unsigned int*) resultv;
  sum += *result;
}

void handle_read_print (void* resultv, void* not_used) {
  handle_read(resultv, not_used);
  printf("%d\n", sum);
  exit(EXIT_SUCCESS);
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: aRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();
  pending_reads = num_blocks;

  int block[num_blocks];

  // Trigger reads and sum blocks
  for (int i = 0; i < num_blocks - 1; i++)
  {
    queue_enqueue(pending_read_queue, &block[i], NULL, handle_read);
    disk_schedule_read(&block[i], i);
  }

  queue_enqueue(pending_read_queue, &block[num_blocks - 1], NULL, handle_read_print);
  disk_schedule_read(&block[num_blocks - 1], num_blocks - 1);
  

  // polling loop so that main doesn't return before all of the reads complete
  while (pending_reads > 0);

  printf ("%d\n", sum);
}

