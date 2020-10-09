#ifndef QUEUE_H_
#define QUEUE_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <semaphore.h>

typedef struct Queue
{
  int size;
  //size of the queue

  char** stringQueue;
  // the char** array malloc'ed pointed to by Queue

  int head;
  // keep track of the first element to be removed from the queue

  int firstAvailable;
  // keep track of the first available slot of the stringQueue that can store

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

  sem_t sem_en;
  sem_t sem_de;
  // semaphores

  sem_t mutex;
  // mutex for critical section
  //*** Can we allow enqueue and dequeue simultaneously?
  // Yes, I think so, because operations do not overlap and
  // the only possibility that head == firstAvailable is when queue is empty
  
} Queue;

int get_time();
Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char* DequeueString(Queue *q);
void PrintQueueStats(Queue *q);


#endif
