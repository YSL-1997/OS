/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "Queue.h"


// Queue initialization with error handling
Queue* CreateStringQueue(int size)
{
  Queue* q = (Queue*)malloc(sizeof(Queue));
  handle_malloc_error(q); // error handling

  q->stringQueue = (char**)malloc(sizeof(char*)*size);
  handle_malloc_error(q->stringQueue); // error handling

  q->size = size;
  q->head = 0;
  q->firstAvailable = 0;
  q->statistics = stats_init();
  // error-handling in stats_init()
  
  handle_sem_init_error(sem_init(&q->sem_en, 0, size));
  handle_sem_init_error(sem_init(&q->sem_de, 0, 0));
  handle_sem_init_error(sem_init(&q->mutex, 0, 1));
  // error handling for sem_init()
  
  return q;
}


/*
  Functionality: places the pointer to the string at the end of queue.
  queue full => this function blocks until there is space available.
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
  Functionality: removes a pointer to a string from beginning of queue q.
  queue empty => blocked until there is a string placed into the queue.
  Return value: the pointer that was removed from the queue.
*/
char* DequeueString(Queue* q)
{
  deq_start(q->statistics); // store the start_deq time
  handle_sem_wait_error(sem_wait(&q->sem_de)); // error handling

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

