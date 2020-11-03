/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

/*
  Writer will write the line to standard output.
  Remember to free after we write the string since
  the string is malloced.
*/

/*

void* func_writer(Queue* q){
  char* str = DequeueString(q);
  printf("Writer Output: %s\n", str);
  free(str);
}
*/

#include "writer.h"

void* func_writer(void* q){
  Queue* x = (Queue*)q;

  while(1){
    char* str = DequeueString(x);
    if(str[0] == '\0') break;
    printf("%s\n", str);
    free(str);
  }
 
  pthread_exit(NULL);
}
