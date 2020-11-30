
/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

/*
  this module reads the trace file line by line, and stores the 
  start line number & end line number for each process, and read input
  arguments to get frame size of RAM
*/

#include <stdio.h>
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include "tsearch.h"

static int MAX_LEN = 4096;

// create a process pointer and initialize with pid 
process* create_process(char* pid)
{
  process* ptr = (process*)malloc(sizeof(process));
  handle_malloc_error(ptr);
  
  ptr->pid = pid;
  ptr->start_index = 0;
  ptr->end_index = 0;
  ptr->cur_index = 0;
  ptr->io_next = NULL;
  ptr->runnable_next = NULL;
  ptr->runnable_prev = NULL;
  ptr->is_blocked = false;
  ptr->timer = 0;
  ptr->blocked_vpn = "";

  return ptr;
}

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
   given a line string, split it into string array
   input: line string, length of the array
   return: a pointer to a list of "words", i.e. the spilitted array
*/
char** parsing(char* string){
  char ch = 'a'; //initialize character variable
  int col_num = 2; // 2 string in string_arr
  int word_index = 0; //initialize the index of string array
  int str_index = 0; //initialize the index of each string
  int read_len = 0; //initialize the index of line string
  int len = strnlen(string, MAX_LEN); //set the max length of each string

  
  // create a pointer to string to store every parsing word
  char* str = (char*)malloc(len * sizeof(char));
  handle_malloc_error(str);
  
  char** string_arr = malloc(col_num * sizeof(char*));
  handle_malloc_error(string_arr);

  while(1){
    ch = string[read_len];

    // if line contains non decimal digit, report an error 
    if(!isdigit(ch)  && !isspace(ch) && ch != '\0'){
      fprintf(stderr, "Line contains non-numeric digit (%d)\n", ch);
      exit(EXIT_FAILURE);
    }
    
    // if ch is the space character or the end of string
    if(ch == ' ' || ch == '\t' || ch =='\0'){
      // check the character before space, if not space character, store the 
      // contiguous string
      if(str_index != 0){
        str[str_index] = '\0';
        string_arr[word_index] = malloc((str_index + 1) * sizeof(char));
	      strncpy(string_arr[word_index], str, str_index+1);
	      word_index++;
	      str_index = 0;
      }

      //if reach the end of line string, free memory and break
      if(ch == '\0' || ch == '\n' ){
	      free(str);
	      break;
      }
      read_len++; 
    }

    // if ch is not space
    else{
      str[str_index] = (char) ch;
      str_index++;
      read_len++; 
    }
  }
 
  // if the line is not empty and word number in string array is not 2 
  if(word_index != 2 && word_index != 0){
    fprintf(stderr, "Line doesn't have exect 2 numbers\n");
    exit(EXIT_FAILURE);
  }

  return string_arr;
}

/* 
   check if a line is blank
   input: a string
   return: true if is blank, false otherwise
*/
bool is_empty(char *s)
{
  while (*s != '\0') {
    if (!isspace(*s))
      return false;
    s++;    
  }
  return true;
}

/*
  Traverse the trace file, create the process tree, find all the PID's, and
  mark the start and end of execution of each process
  input: the head and tail of runnable list of process
  return: processes tree
*/
void* get_processes_info(process** runnable_head, process** runnable_tail) 
{
  char ch = 'a'; //initialize character variable
  int read_len = 0;// index of character of each line
  FILE* fp;
  unsigned long int line_index = 0; //initialize the index of line string
  char** string_arr; //point to the splited string array
  void *root; // pointer to the root of tree
  node_proc *proc_node; //pointer to a process node
  root = NULL;
  process* new_process; //pointer to a new process

  // buffer used to store the line string
  char* buffer = (char*)malloc((MAX_LEN) * sizeof(char));
  handle_malloc_error(buffer);

  fp =  read_file("./smallmix.addrtrace"); 

  while(ch != EOF){
    ch = fgetc(fp);

    // NULL character error
    if(ch == '\0'){
      fprintf(stderr, "NULL character error\n");
      exit(EXIT_FAILURE);
    }

    //if line length exceed the max length, report an error
    if(read_len == MAX_LEN){ 
      buffer[read_len-1] = '\0';
      fprintf(stderr, "Input line is too long\n");
      exit(EXIT_FAILURE);
    }

    if(ch != '\n' && ch != EOF){ 
      buffer[read_len] = (char)ch;
      read_len++;
    }

    // if a new line character or end of file
    else{ 
      buffer[read_len] = '\0';

      // if the trace file is empty
      if(ch == EOF && read_len == 0 && line_index == 0){
	      fprintf(stderr, "%d: <empty file>\n", 1);
	      exit(EXIT_FAILURE); 
      }

       //if the line is blank or comment line, reset index then continue
      if(is_empty(buffer)){
        if(ch == EOF){
          free(buffer);
          break;	          
        }
        else{
          read_len = 0;
          line_index++;
          continue;	          
        }	
      }
      // split the line to string array
      string_arr = parsing(buffer);

      // set offsets of the start and end of each line from the begining of file
      // offset between the end and start is the line length and a '\n' or EOF
      unsigned long start = (unsigned long)(ftell(fp) - strlen(buffer) -1);
      unsigned long end;
      if(ch == '\n'){
        end = (unsigned long)ftell(fp);
      }
      //if the line end with EOF, end of execution of process must be one byte 
      //before the EOF
      else{
        end = (unsigned long)(ftell(fp) -1);
      }
      //check whether the pid of current line is in the process tree
      proc_node = find_proc(&root, string_arr[0]);
     
      //if not found, create a new process and update the fields
      if(proc_node == NULL){
        // create a new process, update fields
        new_process = create_process(string_arr[0]);
        new_process->start_index = start;
        new_process->end_index = end;

        /* using the runnable_pre and runnable_next to create a double lined 
        list for all process */
        // check if length of runnable list is 0
        if(*runnable_head == NULL && *runnable_tail == NULL){
          *runnable_head = new_process;
          *runnable_tail = new_process;
        }
        else{
          // add to (*runnable_tail)->runnable_next and update runnable_tail
          (*runnable_tail)->runnable_next = new_process;
          new_process->runnable_prev = *runnable_tail;
          *runnable_tail = new_process;
        }

        // add a node to process tree
        add_to_proc_table(&root, create_entry_proc(string_arr[0], new_process));
      }
      //if the process exists in process tree, only need to update end_index
      else{
        (proc_node->value)->end_index = end;
      }
      
      // if there is new line, reset index
      if(ch == '\n'){
	      read_len = 0;
	      line_index++; 
      }
      // if reach the end of file, free malloc and break
      else{
	      free(buffer); 
        break;
      } 

    }
  }
  fclose(fp);
  return root;
}

/*
  Get the input page size and memory size, calculate the frame size in memory
*/
unsigned long option_parser( int argc, char* argv[]){
  int ch;
  unsigned long default_page_size = 4096;
  unsigned long page_size; 
  unsigned long memory_size;
  unsigned long default_memory_size = pow(2, 30);
  unsigned long frame_size;

  while ((ch = getopt(argc, argv, "p::m::")) != -1){
    switch (ch)
		{
		case 'p':
			if (optarg == NULL) 
			{
				page_size = default_page_size;
			}
			else
			{
				page_size = (int)strtol(optarg, NULL, 10);
			}
			break;
		case 'm':
			if (optarg == NULL)
			{
				memory_size = default_memory_size;
			}
			else
			{
				memory_size = (unsigned long int)(strtol(optarg, NULL, 10) * default_memory_size);
			}
			break;

    default:
			fprintf(stderr, "Error command");
			exit(EXIT_FAILURE);

    }
  }

  // if any invalid input
  if (optind < argc)
	{
    fprintf(stderr, "Error command");
		exit(EXIT_FAILURE);
  }

  // if input page size is not power of 2, report error
  if(! ((page_size != 0) && ((page_size & (page_size - 1)) == 0) \
      && (page_size <= memory_size))){
        fprintf(stderr, "Page size must be power of 2 and less than memory size");
		    exit(EXIT_FAILURE);
  }
  // set frame size
  frame_size = memory_size/page_size;
  return frame_size;
}


int main(int argc, char* argv[]){
  void* root;

  unsigned long frame_size = option_parser( argc, argv);
  printf("frame_size %lu\n", frame_size);

  node_proc *proc_node;
  process* runnable_head = malloc( sizeof(struct process));
  process* runnable_tail = malloc( sizeof(struct process));
  runnable_head = NULL;
  runnable_tail = NULL;
  root = get_processes_info(&runnable_head, &runnable_tail);

  return 1;
}