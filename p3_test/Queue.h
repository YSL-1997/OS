#ifndef QUEUE_H_
#define QUEUE_H_


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
  // keep track of the first available slot of the stringQueue to enq

  stats* statistics;
  // pointer to a statistics structure
  
  sem_t sem_en;
  sem_t sem_de;
  sem_t mutex;
 
} Queue;

// Queue initialization
Queue *CreateStringQueue(int size);

// Places the pointer to the string at the end of queue.
void EnqueueString(Queue *q, char *string);

// Removes a pointer to a string from beginning of queue q.
char* DequeueString(Queue *q);

// call print_stats() in stats module
void PrintQueueStats(Queue *q);


#endif
