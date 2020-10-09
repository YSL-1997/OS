#include "Queue.h"

// static Queue myQueues[3];

// static sem_t sem_en[3];
// static sem_t sem_en2[3];
// static sem_t sem_en3[3];



int get_time()
{
  struct timeval cur_time;
  gettimeofday(&cur_time, NULL);
  return cur_time.tv_sec;
  
}

/* Dynamically allocate (malloc) a new Queue structure, initialize it with 
   an array of character points of length size.
   Malloc the queue structure and then malloc the char ** array pointed to 
   from that structure
   Remember to any state and synchronization variables used in this structure
   The function returns a pointer to the new queue structure.
   For testing purposes, create your Queue's with a size of 10.
*/
Queue* CreateStringQueue(int size)
{
  Queue* q = (Queue*)malloc(sizeof(Queue));

  q->size = size;
  q->stringQueue = (char**)malloc(sizeof(char*)*size);
  q->head = 0;
  q->firstAvailable = 0;
  q->enqueueCount = 0;
  q->dequeueCount = 0;
  q->enqueueTime = 0;
  q->dequeueTime = 0;

  sem_init(&q->sem_en, 0, size);
  sem_init(&q->sem_de, 0, 0);
  sem_init(&q->mutex, 0, 1);

  return q;
  
}


// This function places the pointer to the string at the end of queue q.
// If the queue is full, then this function blocks until there is space
// available.
void EnqueueString(Queue* q, char* string)
{
  int start_time = get_time();

  sem_wait(&q->sem_en);

  // enter the critical section
  sem_wait(&mutex);
  // I think this critical section is not a must
  q->stringQueue[q->firstAvailable] = string;
  q->firstAvailable = (firstAvailable+1) % q->size;
  q->enqueueCount++;

  // leave the critical section
  sem_post(&mutex);

  sem_post(&q->sem_de);

  // int end_time = get_time();
  q->enqueueTime += get_time() - start_time;
  
}

// This function removes a pointer to a string from the beginning of queue q.
// If the queue is empty, then this function blocks until there is a string
// placed into the queue.
// This function returns the pointer that was removed from the queue.
char* DequeueString(Queue* q){
  int start_time = get_time();

  sem_wait(&q->sem_de);

  // enter the critical section
  sem_wait(&mutex);
  // This critical section is not a must, as far as I'm concerned.
  char* ret_ptr = q->stringQueue[head];
  q->head = (q->head + 1) % q->size;
  q->dequeueCount++;

  // leave the critical section
  sem_post(&mutex);

  sem_post(&q->sem_en);

  q->dequeueTime += get_time() - start_time;
  return ret_ptr;
  
}

// This function prints the statistics for this queue
void PrintQueueStats(Queue* q){
  fprintf(stderr, "The number of enqueues: %d\n", q->enqueueCount);
  fprintf(stderr, "The number of dequeues: %d\n", q->dequeueCount);
  fprintf(stderr, "The total time cost of the enqueue: %d\n", q->enqueueTime);
  fprintf(stderr, "The total time cost of the dequeue: %d\n", q->dequeueTime);
}

