#include <stdlib.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#include "threadpool.h"

struct task {
  void (*fun)(tpool_t, void *);
  void* arg;
  struct task* next;
};

struct tpool {

  uthread_mutex_t mx;
  uthread_cond_t cond;
  uthread_t * threads;
  struct task* task;
  int done;
  int numThreads;
  int tasksLeft;
  int join;
};

/* Function executed by each pool worker thread. This function is
 * responsible for running individual tasks. The function continues
 * running as long as either the pool is not yet joined, or there are
 * unstarted tasks to run. If there are no tasks to run, and the pool
 * has not yet been joined, the worker thread must be blocked.
 * 
 * Parameter: param: The pool associated to the thread.
 * Returns: nothing.
 */
static void *worker_thread(void *param) {
  printf("worker thread \n");
  tpool_t pool = param;

  
  printf("worker thread1 \n");
  while (0==0) {
    uthread_mutex_lock(pool->mx);
    while (pool->tasksLeft == 0)
    {
      if (pool->join && pool->tasksLeft == 0)
      {
        uthread_mutex_unlock(pool->mx);
        tpool_join(pool);
      }
      uthread_cond_wait(pool->cond);
    }
    printf("worker thread2 \n");

    if (pool->task != NULL)
    {
      printf("worker thread3 \n");
      pool->task->fun(pool, pool->task->arg);
      struct task* curr = pool->task;
      pool->task = pool->task->next;
      pool->tasksLeft--;
      free(curr);
      if (pool->tasksLeft == 0)
      {
        pool->done = 1;
      }
      uthread_cond_signal(pool->cond);
    }
    uthread_mutex_unlock(pool->mx);
  }
  
  
  return NULL;
}

/* Creates (allocates) and initializes a new thread pool. Also creates
 * `num_threads` worker threads associated to the pool, so that
 * `num_threads` tasks can run in parallel at any given time.
 *
 * Parameter: num_threads: Number of worker threads to be created.
 * Returns: a pointer to the new thread pool object.
 */
tpool_t tpool_create(unsigned int num_threads) {

  tpool_t pool = malloc(sizeof(struct tpool));
  printf("creating new pool \n");
  pool->numThreads = num_threads;
  pool->tasksLeft = 0;
  pool->done = 0;

  pool->task = NULL;

  pool->threads = malloc(sizeof(uthread_t)*num_threads);

  
  pool->mx = uthread_mutex_create();
  pool->cond = uthread_cond_create(pool->mx);

  for (size_t i = 0; i < num_threads; i++) {
    pool->threads[i] = uthread_create(worker_thread, pool);
  }

  pool->join = 0;

  return pool;
}

/* Queues a new task, to be executed by one of the worker threads
 * associated to the pool. The task is represented by function `fun`,
 * which receives the pool and a generic pointer as parameters. If any
 * of the worker threads is available, `fun` is started immediately by
 * one of the worker threads. If all of the worker threads are busy,
 * `fun` is scheduled to be executed when a worker thread becomes
 * available. Tasks are retrieved by individual worker threads in the
 * order in which they are scheduled, though due to the nature of
 * concurrency they may not start exactly in the same order. This
 * function returns immediately, and does not wait for `fun` to
 * complete.
 *
 * Parameters: pool: the pool that is expected to run the task.
 *             fun: the function that should be executed.
 *             arg: the argument to be passed to fun.
 */
void tpool_schedule_task(tpool_t pool, void (*fun)(tpool_t, void *),
                         void *arg) {
  
  tpool_t tpool = pool;
  printf("scheduling new task \n");

  struct task* newTask = malloc(sizeof(struct task));
  newTask->fun = fun;
  newTask->arg = arg;
  newTask->next = NULL;

  uthread_mutex_lock(tpool->mx);


  if (tpool->task == NULL)
  {
    tpool->task = newTask;
  } else 
  {
    struct task* curr = tpool->task;
    while (curr->next != NULL)
    {
      curr = curr->next;
    }
    curr->next = newTask;
  }
  tpool->tasksLeft++;
  uthread_cond_signal(tpool->cond);
  uthread_mutex_unlock(tpool->mx);
  
}

/* Blocks until the thread pool has no more scheduled tasks; then,
 * joins all worker threads, and frees the pool and all related
 * resources. Once this function returns, the pool cannot be used
 * anymore.
 *
 * Parameters: pool: the pool to be joined.
 */
void tpool_join(tpool_t pool) {

  printf("joining pool \n");

  tpool_t tpool = pool;
  tpool->join = 1;
  uthread_mutex_lock(pool->mx);
  while (tpool->tasksLeft == 0 && pool->done != 1) {
    uthread_cond_wait(tpool->cond);
  }
  uthread_mutex_unlock(tpool->mx);

  for(int i = 0; i < tpool->numThreads; i++) {
		uthread_join(tpool->threads[i], NULL);
	}

  uthread_mutex_destroy(tpool->mx);
  uthread_cond_destroy(tpool->cond);
  free(tpool->threads);
  free(tpool);
  exit(EXIT_SUCCESS);
}
