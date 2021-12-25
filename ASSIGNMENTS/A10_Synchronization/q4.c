#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_t t0, t1, t2;
uthread_mutex_t mutex1;
uthread_cond_t cond1, cond2;
int done1, done2;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void* p0(void* v) {
  uthread_mutex_lock(mutex1);
  randomStall();
  printf("zero\n");
  done1 = 1;
  uthread_cond_signal(cond1);
  uthread_mutex_unlock(mutex1);
  return NULL;
}

void* p1(void* v) {
  uthread_mutex_lock(mutex1);
  randomStall();
  if (done1 == 0)
  {
    uthread_cond_wait(cond1);
  }
  printf("one\n");
  done2 = 1;
  uthread_cond_signal(cond2);
  uthread_mutex_unlock(mutex1);
  return NULL;
}

void* p2(void* v) {
  uthread_mutex_lock(mutex1);
  randomStall();
  if (done2 == 0)
  {
    uthread_cond_wait(cond2);
  }
  printf("two\n");
  uthread_mutex_unlock(mutex1);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mutex1 = uthread_mutex_create();
  cond1 = uthread_cond_create(mutex1);
  cond2 = uthread_cond_create(mutex1);
  t0 = uthread_create(p0, NULL);
  t1 = uthread_create(p1, NULL);
  t2 = uthread_create(p2, NULL);
  randomStall();
  uthread_join (t0, NULL);
  uthread_join (t1, NULL);
  uthread_join (t2, NULL);
  printf("three\n");
  printf("------\n");
}
