/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "stats.h"

stats* stats_init(){
  stats* s = (stats*)malloc(sizeof(stats));
  handle_malloc_error(s); // error handling

  s->enq_start_time = 0;
  s->enqueueCount = 0;
  s->dequeueCount = 0;
  s->enqueueTime = 0;
  s->dequeueTime = 0;
  handle_sem_init_error(sem_init(&s->mutex, 0, 1));
  
  return s;
}

// get the current time
double get_time()
{
  struct timeval cur_time;
  handle_gettime_error(gettimeofday(&cur_time, NULL));
  // error handling
  double sec = cur_time.tv_sec;
  double microsec = cur_time.tv_usec;
  
  return sec + microsec/1000000; 
}

// increment enqueueCount by 1
void enq_inc(stats* s)
{
  handle_sem_wait_error(sem_wait(&s->mutex));
  s->enqueueCount++;
  handle_sem_post_error(sem_post(&s->mutex));
}

// increment dequeueCount by 1
void deq_inc(stats* s)
{
  handle_sem_wait_error(sem_wait(&s->mutex));
  s->dequeueCount++;
  handle_sem_post_error(sem_post(&s->mutex));
}

// record the enqueue start time
void enq_start(stats* s)
{
  handle_sem_wait_error(sem_wait(&s->mutex));
  s->enq_start_time = get_time();
  handle_sem_post_error(sem_post(&s->mutex));
}

// record the dequeue start time
void deq_start(stats* s)
{
  handle_sem_wait_error(sem_wait(&s->mutex));
  s->deq_start_time = get_time();
  handle_sem_post_error(sem_post(&s->mutex));
}

// update enqueueTime
void enq_end(stats* s)
{
  handle_sem_wait_error(sem_wait(&s->mutex));
  s->enqueueTime += get_time() - s->enq_start_time;
  handle_sem_post_error(sem_post(&s->mutex));
}

// update dequeueTime
void deq_end(stats* s)
{
  handle_sem_wait_error(sem_wait(&s->mutex));
  s->dequeueTime += get_time() - s->deq_start_time;
  handle_sem_post_error(sem_post(&s->mutex));
}

// print Queue status
void print_stats(stats* s){
  // the while spaces are for readability
  fprintf(stderr,
	  "  The number of enqueues: %d\n",s->enqueueCount);
  fprintf(stderr,
	  "  The number of dequeues: %d\n", s->dequeueCount);
  fprintf(stderr,
	  "  The total time cost of the enqueue: %Lf\n", s->enqueueTime);
  fprintf(stderr,
	  "  The total time cost of the dequeue: %Lf\n", s->dequeueTime);
  fprintf(stderr, "\n");
}
