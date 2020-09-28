/*
	Authors:
	Yusen Liu - NetID: liu797, CS login: yusen
	Yuting Yan - NetID: yan86 , CS login: yuting
	Project #2 - Shared Memory Producer/Consumer Program
*/

/*
	The Reader thread will read from standard input, one line at a time. 
	Reader will take the each line of the input and pass it to thread Munch1 through a queue of character strings.
	
	Each time that the Reader thread is going to read a string from standard input, it will first malloc memory for that string. 
	The pointer to that string will then be passed down the queues between the threads until it finally arrives at the Writer thread. 
	After Writer writes out the string pointed to by the pointer, it will then free the memory that holds the string.
*/
