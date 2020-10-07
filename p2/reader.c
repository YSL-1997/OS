#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

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

    can only store 4096 chars
      since the last one should be '\0', we can only store 4095(MAX_LEN-1)
      0 1 2 3 4 5 ... i (i max is 4095), so before \0, i can be at most (MAX_LEN-2)
      meaning that we
    
    if(read_len >= MAX_LEN){
      buffer[read_len-1] = '\0';
      fprintf(stderr, "Input line too long.\n");
      
      while(ch != '\n' && ch != EOF){
	ch = getc(fp) ;   
      }
      read_len = 0; // reset i to be 0
      continue;
    }
    
    buffer[read_len] = (char)ch;
    read_len++;
    
    if( ch == '\n' || ch == EOF){
      buffer[read_len] = '\0';
      // now, we have that buffer[0~i] is valid
      // need to put into a new malloc'ed str.
      char* ret_str = malloc((read_len+1) * sizeof(char));
      strncpy(ret_str, buffer, read_len+1); // store what's in buffer to ret_str
      EnqueueString(q, ret_str);
      read_len = 0;
      if(ch == EOF)
	break;
    }
  }

  // read finished, enqueue a null char
  EnqueueString(q, "\0");
  // not sure if this will work, if not, try replacing \0 with NULL.
  
  fclose(fp);
  free(buffer);

  // pthread_exit(0);
}


