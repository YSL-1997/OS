/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

/*
  this module reads the trace file line by line, and stores the 
  start line number & end line number for each process
*/

#include <stdio.h>

/*
  this function reads a file according to path
  return: file pointer
*/
FILE* read_file(char* path)
{
  FILE* fp = fopen(path, "r");
  if(fp == NULL){
    fprintf(stderr, "file open failed\n");
    exit(EXIT_FAILURE);
  }
  
  return fp;
}

/*
  this function traverses the file 
  return: array of processes 
*/
processes* get_processes_info(FILE* fp)
{
  char ch = 'a';
  while(ch != EOF){
    ch = fgets(fp);
    
  }
}
