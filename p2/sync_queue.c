/*
  Yusen Liu, liu797@wis
Since your program is reading in strings of characters and passing them from thread to thread, your queue will be a queue of pointers to strings (char *).
Queues are dynamically created. So, for this assignment, you will need one between each pair of threads (so you'll need three queues).

Queues will be represented by a structure of type Queue.

The main external functions for this module should be:

Queue *CreateStringQueue(int size)
Dynamically allocate a new Queue structure and initialize it with an array of character points of length size. That means you'll malloc the queue structure and then malloc the char ** array pointed to from that structure. Also remember to any state and synchronization variables used in this structure.

The function returns a pointer to the new queue structure.

For testing purposes, create your Queue's with a size of 10.

void EnqueueString(Queue *q, char *string)
This function places the pointer to the string at the end of queue q. If the queue is full, then this function blocks until there is space available.

char * DequeueString(Queue *q)
This function removes a pointer to a string from the beginning of queue q. If the queue is empty, then this function blocks until there is a string placed into the queue. This function returns the pointer that was removed from the queue.

void PrintQueueStats(Queue *q)
This function prints the statistics for this queue (see the next section for details).
