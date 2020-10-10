#ifndef STATS_H_
#define STATS_H_


#include <sys/time.h>
#include "errorHandling.h"

typedef struct stats{
  int enq_start_time;
  // keep track of each enq_operation's start time

  int deq_start_time;
  // keep track of each deq_operation's start time

  int enqueueCount;
  // A count of the number of strings enqueued on this queue.

  int dequeueCount;
  // A count of the number of strings dequeued on this queue. We would
  // expect that when the program exits, the two count values are equal.

  int enqueueTime;
  // use gettimeofday
  // The amount of elapsed (wallclock) time that a thread spent trying to
  // do an enqueue. You will record the time at the start of the
  // EnqueueString function and again at the end of the function,
  // then take the difference.

  int dequeueTime;
  /*
      The amount of elapsed (wallclock) time that a thread spent trying to do
      an dequeue. You will record the time at the start of the DequeueString
      function and again at the end of the function, then take the difference.
  */
}stats;

// stats module initialization
stats* stats_init();

// get the current time
int get_time();

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

void print_stats(stats* s);

#endif
