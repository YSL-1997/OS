#ifndef STATS_H_
#define STATS_H_


#include <sys/time.h>
#include <semaphore.h>
#include "errorHandling.h"

typedef struct stats{
  long double enq_start_time;
  // Keep track of each enq_operation's start time.

  long double deq_start_time;
  // Keep track of each deq_operation's start time.

  int enqueueCount;
  // A count of the number of strings enqueued on this queue.

  int dequeueCount;
  // A count of the number of strings dequeued on this queue.

  long double enqueueTime;
  // Amount of time - a thread spent trying to do an enqueue. 

  long double dequeueTime;
  // Amount of time - a thread spent trying to do an dequeue.

  sem_t mutex;
  // semaphore used when updating
  
}stats;


stats* stats_init();
double get_time();
void enq_inc(stats* s);
void deq_inc(stats* s);
void enq_start(stats* s);
void deq_start(stats* s);
void enq_end(stats* s);
void deq_end(stats* s);
void print_stats(stats* s);

#endif
