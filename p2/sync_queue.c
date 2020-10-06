/*
	Authors:
	Yusen Liu - NetID: liu797, CS login: yusen
	Yuting Yan - NetID: yan86 , CS login: yutingy
	Project #2 - Shared Memory Producer/Consumer Program
*/

/*
Since your program is reading in strings of characters and passing them from 
thread to thread, your queue will be a queue of pointers to strings (char *).
Queues are dynamically created. So, for this assignment, you will need one 
between each pair of threads (so you'll need three queues).
Queues will be represented by a structure of type Queue.
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// isBlocked probably needed to be blocked_by_en and blocked_by_de
// stringArray implementation (consider firstAvailable and head) can be two
// ways - circular array or linear array


struct Queue
{
	int size;
	//size of the queue

	char** stringArray;
	// the char** array malloc'ed pointed to by Queue

	int firstAvailable;
	// stores the position of the first available slot of the stringArray

	int isBlocked;
	// 1 - blocked; 0 - not blocked

	int enqueueCount;
	// A count of the number of strings enqueued on this queue.

	int dequeueCount;
	// A count of the number of strings dequeued on this queue. We would
        // expect that when the program exits, the two count values are equal.

	int enqueueBlockCount;
	// A count of the number of times that an enqueue was attempted but
        // blocked.

	int dequeueBlockCount;
	// A count of the number of times that a dequeue was attempted but
        // blocked.
};

// Dynamically allocate (malloc) a new Queue structure, initialize it with an array of character points of length size.
// malloc the queue structure and then malloc the char ** array pointed to from that structure
// remember to any state and synchronization variables used in this structure
// The function returns a pointer to the new queue structure.
// For testing purposes, create your Queue's with a size of 10.
Queue *CreateStringQueue(int size){
	Queue* returnQueue = (Queue*)malloc(sizeof(Queue));
	returnQueue->size = size;
	returnQueue->stringArray = (char**)malloc(sizeof(char**)*size);
	returnQueue->firstAvailable = 0;
	returnQueue->isBlocked = 0;
	returnQueue->enqueueCount = 0;
	returnQueue->dequeueCount = 0;
	returnQueue->enqueueBlockCount = 0;
	returnQueue->dequeueBlockCount = 0;
	return returnQueue;
}



// This function places the pointer to the string at the end of queue q.
// If the queue is full, then this function blocks until there is space available.
void EnqueueString(Queue *q, char *string){
	while(q->firstAvailable == 10){ // the queue is full
		q->isBlocked = 1;
		q->enqueueBlockCount++;
		if(q->isBlocked == 0){ 
			break;
		}
	}
	// now the queue is not full
	strncpy(q->stringArray[firstAvailable], string, strlen(string));
	q->firstAvailable++;
	q->enqueueCount++;

}

// This function removes a pointer to a string from the beginning of queue q. 
// If the queue is empty, then this function blocks until there is a string placed into the queue. 
// This function returns the pointer that was removed from the queue.
char * DequeueString(Queue *q){
	while(q->firstAvailable == 0){
		q->isBlocked
	}
}

// This function prints the statistics for this queue
void PrintQueueStats(Queue *q){
	
}




