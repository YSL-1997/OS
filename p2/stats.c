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

  s->enqueueCount = 0;
  s->dequeueCount = 0;
  s->enqueueTime = 0;
  s->dequeueTime = 0;

  return s;
}

// get the current time
int get_time()
{
  struct timeval cur_time;
  handle_gettime_error(gettimeofday(&cur_time, NULL));
  // error handling
  
  return cur_time.tv_sec; 
}

// increment enqueueCount by 1
void enq_inc(stats* s)
{
  s->enqueueCount++;
}

// increment dequeueCount by 1
void deq_inc(stats* s)
{
  s->dequeueCount++;
}

// record the enqueue start time
void enq_start(stats* s)
{
  s->enq_start_time = get_time();
}

// record the dequeue start time
void deq_start(stats* s)
{
  s->deq_start_time = get_time();
}

// update enqueueTime
void enq_end(stats* s)
{
  s->enqueueTime += get_time() - s->enq_start_time;
}

// update dequeueTime
void deq_end(stats* s)
{
  s->dequeueTime += get_time() - s->deq_start_time;
}

// print Queue status
void print_stats(stats* s){
  // the while spaces are for readability
  fprintf(stderr,
	  "    The number of enqueues: %d\n",s->enqueueCount);
  fprintf(stderr,
	  "    The number of dequeues: %d\n", s->dequeueCount);
  fprintf(stderr,
	  "    The total time cost of the enqueue: %d\n", s->enqueueTime);
  fprintf(stderr,
	  "    The total time cost of the dequeue: %d\n", s->dequeueTime);
  fprintf(stderr, "\n");
}
