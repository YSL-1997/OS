/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include "Queue.h"

const size_t MAX_LEN = 4096;

void func_reader(void* arg){
  Queue* q = (Queue*)arg;
  FILE *fp;
  char ch = 'a'; // in order to enter the while loop
  int read_len = 0; // index of current length

  char *buffer = malloc(MAX_LEN * sizeof(char));
  
  if((fp = fopen("bigfile.txt", "r")) == NULL){
    perror("Could not get info from stdin");
    exit(EXIT_FAILURE);
  }
  
  while(ch != EOF){
    ch = fgetc(fp);

    if(read_len == MAX_LEN-1){ // abort this line
      fprintf(stderr, "Input line too long.\n");
      
      while(1){
	if(ch == '\n')
	  break;
	
	if(ch == EOF){
	  printf("The rest of the line ends with EOF, reader stop\n");
	  exit(0);
	}
	ch = fgetc(fp);
      }

      read_len = 0; // reset i to be 0
      continue;
    }
    
    if(ch == EOF){
      if(read_len == 0){
	printf("The only thing that we have read is EOF\n");
	exit(0);
      }
      else{
	buffer[read_len] = '\0';
	// now, we have that buffer[0 ~ read_len+1] is valid
	// need to put into a new malloc'ed str.
	char* ret_str = malloc((read_len+1) * sizeof(char));
	strncpy(ret_str, buffer, read_len+1); // store what's in buffer to ret_str
	EnqueueString(q, ret_str);
      }
      break;
    }
    
    buffer[read_len] = (char)ch;
    read_len++;
    
    if((char)ch == '\n'){
      buffer[read_len] = '\0';
      // now, we have that buffer[0 ~ read_len+1] is valid
      // need to put into a new malloc'ed str.
      char* ret_str = malloc((read_len+1) * sizeof(char));
      strncpy(ret_str, buffer, read_len+1); // store what's in buffer to ret_str
      EnqueueString(q, ret_str);
      read_len = 0; // reset index
      continue;
    }
  }

  // read finished, enqueue a null char
  EnqueueString(q, "\0");
  // not sure if this will work, if not, try replacing \0 with NULL.
  
  fclose(fp);
  free(buffer);
  pthread_exit(0);
}
