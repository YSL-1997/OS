/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "Queue.h"

/*
  Queue initialization with error handling
  Return value: a pointer to a Queue
*/
Queue* CreateStringQueue(int size)
{
  Queue* q = (Queue*)malloc(sizeof(Queue));
  handle_malloc_error(q); // error handling

  q->stringQueue = (char**)malloc(sizeof(char*)*size);
  handle_malloc_error(q->stringQueue); // error handling

  q->size = size;
  q->head = 0;
  q->firstAvailable = 0;

  // error-handling implemented in stats_init()
  q->statistics = stats_init();

  // error handling for sem_init()
  handle_sem_init_error(sem_init(&q->sem_en, 0, size));
  handle_sem_init_error(sem_init(&q->sem_de, 0, 0));
  handle_sem_init_error(sem_init(&q->mutex, 0, 1));
  
  return q;
}


/*
  Places the pointer to the string at the end of queue.
  queue full => this function blocks until space is available.
*/
void EnqueueString(Queue* q, char* string)
{
  enq_start(q->statistics); // store the start_enq time
  handle_sem_wait_error(sem_wait(&q->sem_en)); // error handling
  
  handle_sem_wait_error(sem_wait(&q->mutex)); 
  q->stringQueue[q->firstAvailable] = string; // store the string
  q->firstAvailable = (q->firstAvailable+1) % q->size; // update index
  enq_inc(q->statistics); // update enqueueCount
  handle_sem_post_error(sem_post(&q->mutex));
  
  handle_sem_post_error(sem_post(&q->sem_de));
  enq_end(q->statistics); // update enqueueTime
}


/*
  Removes a pointer to a string from beginning of queue q.
  queue empty => blocked until a string is enq'ed to the queue.
  Return value: the pointer that was removed from the queue.
*/
char* DequeueString(Queue* q)
{
  deq_start(q->statistics);
  handle_sem_wait_error(sem_wait(&q->sem_de));

  handle_sem_wait_error(sem_wait(&q->mutex));
  char* ret_ptr = q->stringQueue[q->head]; // get the string
  q->head = (q->head + 1) % q->size; // update index
  deq_inc(q->statistics); // uodate dequeueCount
  handle_sem_post_error(sem_post(&q->mutex));

  handle_sem_post_error(sem_post(&q->sem_en));
  deq_end(q->statistics);

  return ret_ptr;
}

// This function prints the statistics for this queue
void PrintQueueStats(Queue* q)
{
  print_stats(q->statistics);
}

