/*
	Authors:
	Yusen Liu - NetID: liu797, CS login: yusen
	Yuting Yan - NetID: yan86 , CS login: yutingy
	Project #2 - Shared Memory Producer/Consumer Program
*/

/*
	The Reader thread will read from standard input, one line at a time. 
	Reader will take the each line of the input and pass it to thread Munch1 through a queue of character strings.
	
	Each time that the Reader thread is going to read a string from standard input, it will first malloc memory for that string. 
	The pointer to that string will then be passed down the queues between the threads until it finally arrives at the Writer thread. 
	After Writer writes out the string pointed to by the pointer, it will then free the memory that holds the string.
*/

/*
	ssize_t getline(char **lineptr, size_t *n, FILE *stream);
	
	getline()  reads  an  entire  line  from  stream,  storing  the address of the buffer containing the text into *lineptr.
		The buffer is null-terminated and includes the newline character, if one was found.
	If *lineptr is set to NULL and *n is set 0 before the call, then getline() will allocate a buffer for  storing the line.
		This buffer should be freed by the user program even if getline() failed.
	Alternatively,  before  calling  getline(),  *lineptr can contain a pointer to a malloc(3)-allocated buffer *n bytes in size.
		If the buffer is not large enough to hold the line, getline() resizes it with realloc(3),  up‐dating *lineptr and *n as necessary.
*/




#include <stdio.h>
#include <stdlib.h>

// not the correct implementation, but one way of using getline()
int main(int argc, char* argv[]){
	char* line = NULL; // used to store
	size_t len = 0;
	ssize_t num_read; // number of characters read from std_in

	printf("Please enter a line:\n");

	while((num_read = getline(&line, &len, stdin)) != -1){  // getline() allocates memory to the buffer - line
		printf("%s\n", line);
	}
	free(line); // remember to free after the bufer - line has been used
	return 0; 
}