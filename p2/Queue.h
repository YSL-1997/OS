#ifndef QUEUE_H_
#define QUEUE_H_


#include <semaphore.h>
#include "stats.h"

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

  stats* statistics;
  // pointer to a statistics structure
  
  sem_t sem_en;
  sem_t sem_de;
  // semaphores

  sem_t mutex;
  // mutex for critical section
  // *** Can we allow enqueue and dequeue simultaneously?
  // Yes, I think so, because operations do not overlap
} Queue;


Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char* DequeueString(Queue *q);
void PrintQueueStats(Queue *q);


#endif
