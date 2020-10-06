#include <stdio.h>
// #define MAX_LEN 4096

const size_t MAX_LEN = 4096;

int main(){
  FILE* fp = fopen("test.txt", "r");

  char c;
  int read_len = 0;
  char* buffer = (char*)malloc(MAX_LEN);
  //////////////////////////////////////// every read, buffer size=?
  
  if(fp == NULL){
    perror("Error in opening file\n");
    return 1;
  }
  
  while(1){
    if(feof(fp)){
      // put the things after \n before EOF into buffer, return
      
      break;
    }

    c = fgetc(fp); // get what we have read
    buffer[read_len] = c; // store into the buffer
    if(buffer[read_len] == '\n'){
      // add the buffer[0 to read_len] to the queue
      // read_len = 0;
      // continue;
    }
    
    if(read_len == max_len){
      // meaning that we've been reading a line that has
      // NOT yet met \n in the buffer
      // if yes, we cannot put what we've stored in buffer into queue
      //    instead, we should keep reading, until we get the \n
      //    then, set read_len to 0;
      //          and go on for the next new read
      do{
	c = fgetc(fp);
	if(c == '\n'){
	  read_len = 0;
	  continue;
	}
      }while(c != '\n');
    }
    
    read_len++;
    
    // printf("%c", c);
  }
  fclose(fp);
  return 0;
}
