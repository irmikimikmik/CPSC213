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

void interrupt_service_routine () {
  void* t;
  queue_dequeue(pending_read_queue, &t, NULL, NULL);
  uthread_unblock(t);
}

void* read_block (void* blocknov) {
  // TODO schedule read and the update (correctly)
  int* blockno = blocknov;
  int value;
  uthread_t t = uthread_self();
  queue_enqueue(pending_read_queue, t, NULL, NULL);
  disk_schedule_read(&value, *blockno);
  uthread_block(t);
  sum += value;

  return NULL;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: tRead num_blocks";
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

  int blocks[num_blocks];
  uthread_t threads[num_blocks];

  // Sum Blocks
  for (size_t i = 0; i < num_blocks; i++)
  {
    blocks[i] = i;
    threads[i] = uthread_create(read_block, &blocks[i]);
  }

  for (int i = 0; i < num_blocks; i++) {
  	uthread_join(threads[i], NULL);
  }

  printf ("%d\n", sum);
}

