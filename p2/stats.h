#ifndef STATS_H_
#define STATS_H_


#include <sys/time.h>
#include <semaphore.h>
#include "errorHandling.h"

typedef struct stats{
  double enq_start_time;
  // Keep track of each enq_operation's start time.

  double deq_start_time;
  // Keep track of each deq_operation's start time.

  int enqueueCount;
  // A count of the number of strings enqueued on this queue.

  int dequeueCount;
  // A count of the number of strings dequeued on this queue.

  double enqueueTime;
  // Amount of time - a thread spent trying to do an enqueue. 

  double dequeueTime;
  // Amount of time - a thread spent trying to do an dequeue.

  sem_t mutex;
  // semaphore used when updating
  
}stats;


// stats module initialization
stats* stats_init();

// get the current time
double get_time();

// increment enqueueCount by 1
void enq_inc(stats* s);

// increment dequeueCount by 1
void deq_inc(stats* s);

// record the enqueue start time
void enq_start(stats* s);

// record the dequeue start time
void deq_start(stats* s);

// update enqueueTime
void enq_end(stats* s);

// update dequeueTime
void deq_end(stats* s);

// print the statistics
void print_stats(stats* s);

#endif
