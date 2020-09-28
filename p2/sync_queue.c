/*
	Authors:
	Yusen Liu - NetID: liu797, CS login: yusen
	Yuting Yan - NetID: yan86 , CS login: yutingy
	Project #2 - Shared Memory Producer/Consumer Program
*/

/*
Since your program is reading in strings of characters and passing them from thread to thread, your queue will be a queue of pointers to strings (char *).
Queues are dynamically created. So, for this assignment, you will need one between each pair of threads (so you'll need three queues).
Queues will be represented by a structure of type Queue.
*/



// Dynamically allocate (malloc) a new Queue structure, initialize it with an array of character points of length size.
// malloc the queue structure and then malloc the char ** array pointed to from that structure
// remember to any state and synchronization variables used in this structure
// The function returns a pointer to the new queue structure.
// For testing purposes, create your Queue's with a size of 10.
Queue *CreateStringQueue(int size){
	
}



// This function places the pointer to the string at the end of queue q.
// If the queue is full, then this function blocks until there is space available.
void EnqueueString(Queue *q, char *string){
	
}

// This function removes a pointer to a string from the beginning of queue q. 
// If the queue is empty, then this function blocks until there is a string placed into the queue. 
// This function returns the pointer that was removed from the queue.
char * DequeueString(Queue *q){
	
}

// This function prints the statistics for this queue
void PrintQueueStats(Queue *q){
	
}

