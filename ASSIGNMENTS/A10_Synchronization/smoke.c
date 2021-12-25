#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked


struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->match   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  return agent;
}

struct Smoker {
  struct SmokerPool* pool;
  int typeOfSmoker;
};

struct SmokerPool {
  struct Agent* agent;
  int tobacco;
  int paper;
  int match;
};

struct Smoker* createSmoker(int typeOfSmoker, struct SmokerPool* pool){
  struct Smoker* s = malloc(sizeof(struct Smoker));
  s->typeOfSmoker = typeOfSmoker;
  s->pool = pool;
  return s;
}

struct SmokerPool* createSmokerPool(struct Agent* agent){
  struct SmokerPool* sp = malloc (sizeof (struct SmokerPool));
  sp->agent = agent;
  sp->tobacco = 0;
  sp->paper = 0;
  sp->match = 0;
  return sp;
}

void* smoker (void* input){
  struct Smoker* s = input;
  struct SmokerPool* sp = s->pool;
  struct Agent* a = sp->agent;
  uthread_mutex_lock(a->mutex);

  while(1){

    if (s->typeOfSmoker == PAPER){

      uthread_cond_wait(a->paper);
      if (sp->match > 0 && sp->tobacco > 0) {
        smoke_count[s->typeOfSmoker]++;
        uthread_cond_signal(a->smoke);
        sp->match--;
        sp->tobacco--;
      } else {
        sp->paper++;
      }

    } else if (s->typeOfSmoker == MATCH){

      
      uthread_cond_wait(a->match);
      if (sp->paper > 0 && sp->tobacco > 0){
        smoke_count[s->typeOfSmoker]++;
        uthread_cond_signal(a->smoke);
        sp->paper--;
        sp->tobacco--;
      } else {
        sp->match++;
      }

    } else if (s->typeOfSmoker == TOBACCO){

      uthread_cond_wait(a->tobacco);
      if (sp->paper > 0 && sp->match > 0){
        smoke_count[s->typeOfSmoker]++;
        uthread_cond_signal(a->smoke);
        sp->paper--;
        sp->match--;
      } else {
        sp->tobacco++;
      }
    }

    if (sp->paper > 0 && sp->tobacco > 0) {
      uthread_cond_signal(a->match);
    } else if (sp->paper > 0 && sp->match > 0) {
      uthread_cond_signal(a->tobacco);
    } else if (sp->tobacco > 0 && sp->match > 0) {
      uthread_cond_signal(a->paper);
    }

  }
  uthread_mutex_unlock(a->mutex);
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it does
 * is choose 2 random resources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));
  
  uthread_mutex_lock (a->mutex);
  // Wait until all other threads are waiting for a signal
  while (num_active_threads < 3)
    uthread_cond_wait (a->smoke);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 6;
    switch(r) {
    case 0:
      signal_count[TOBACCO]++;
      printf ("match available\n");
      uthread_cond_signal (a->match);
      printf ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    case 1:
      signal_count[PAPER]++;
      printf ("match available\n");
      uthread_cond_signal (a->match);
      printf ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 2:
      signal_count[MATCH]++;
      printf ("paper available\n");
      uthread_cond_signal (a->paper);
      printf ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 3:
      signal_count[TOBACCO]++;
      printf ("paper available\n");
      uthread_cond_signal (a->paper);
      printf ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 4:
      signal_count[PAPER]++;
      printf ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      printf ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 5:
      signal_count[MATCH]++;
      printf ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      printf ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    }
    printf ("agent is waiting for smoker to smoke\n");
    uthread_cond_wait (a->smoke);
  }
  
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  
  struct Agent* a = createAgent();
  uthread_t agent_thread;
  struct SmokerPool* sp = createSmokerPool(a);

  uthread_init(5);

  uthread_t match   = uthread_create(smoker, createSmoker(MATCH,   sp));
  num_active_threads++;
  uthread_t paper   = uthread_create(smoker, createSmoker(PAPER,   sp));
  num_active_threads++;
  uthread_t tobacco = uthread_create(smoker, createSmoker(TOBACCO, sp));
  num_active_threads++;
  
  agent_thread = uthread_create(agent, a);
  uthread_join(agent_thread, NULL);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);

  return 0;
}
