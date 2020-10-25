/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - Make
*/

#include <stdio.h>
#include <stdlib.h>


/*
  A function that reads a file "path" and returns the file pointer
  Return value: a pointer to a file specified by "path"
  On failure, returns NULL
*/
FILE* read_file(char* path)
{
  FILE* fp = fopen(path, 'r');

  if(fp == NULL)
    fprintf(stderr, "Cannot open file: %s\n", path);
  else
    return fp;
}


/*
  for each line we read, process it
*/






/*
  parse the whole file "path"
*/

void parse(char* path){
  FILE* fp;
  
  if((fp = read_file("./makefile")) == NULL){ // makefile not found
    if((fp = read_file("./Makefile")) == NULL){ // Makefile not found
      fprintf(stderr, "makefile, Makefile not found\n");
      exit(1);
    }
  }
  
}