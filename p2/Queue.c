/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "Queue.h"


/* Dynamically allocate (malloc) a new Queue structure, initialize it with 
   an array of character points of length size.
   Malloc the queue structure and then malloc the char ** array pointed to 
   from that structure
   Remember to any state and synchronization variables used in this structure
   The function returns a pointer to the new queue structure.
   For testing purposes, create your Queue's with a size of 10.
*/

// Queue initialization with error handling
Queue* CreateStringQueue(int size)
{
  Queue* q = (Queue*)malloc(sizeof(Queue));
  handle_malloc_error(q);

  q->stringQueue = (char**)malloc(sizeof(char*)*size);
  handle_malloc_error(q->stringQueue);

  q->size = size;
  q->head = 0;
  q->firstAvailable = 0;
  q->statistics = stats_init();
  // error-handling in stats_init()
  
  handle_sem_init_error(sem_init(&q->sem_en, 0, size));
  handle_sem_init_error(sem_init(&q->sem_de, 0, 0));
  handle_sem_init_error(sem_init(&q->mutex, 0, 1));

  return q;
}


// This function places the pointer to the string at the end of queue q.
// If the queue is full, then this function blocks until there is space
// available.
void EnqueueString(Queue* q, char* string)
{
  enq_start(q->statistics);
  handle_sem_wait_error(sem_wait(&q->sem_en));
  
  handle_sem_wait_error(sem_wait(&q->mutex)); 
  q->stringQueue[q->firstAvailable] = string; // store the string
  q->firstAvailable = (q->firstAvailable+1) % q->size; // update index
  enq_inc(q->statistics); // update enqueueCount
  handle_sem_post_error(sem_post(&q->mutex));

  handle_sem_post_error(sem_post(&q->sem_de));
  enq_end(q->statistics); // update enqueueTime
}

// This function removes a pointer to a string from the beginning of queue q.
// If the queue is empty, then this function blocks until there is a string
// placed into the queue.
// This function returns the pointer that was removed from the queue.
char* DequeueString(Queue* q)
{
  deq_start(q->statistics);
  handle_sem_wait_error(sem_wait(&q->sem_de));

  handle_sem_wait_error(sem_wait(&q->mutex));
  char* ret_ptr = q->stringQueue[q->head]; // get the string
  q->head = (q->head + 1) % q->size; // update index
  deq_inc(q->statistics); // update dequeueCount
  handle_sem_post_error(sem_post(&q->mutex));

  handle_sem_post_error(sem_post(&q->sem_en));
  deq_end(q->statistics); // update dequeueTime

  return ret_ptr;
}

// This function prints the statistics for this queue
void PrintQueueStats(Queue* q)
{
  print_stats(q->statistics);
}

