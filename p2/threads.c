/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "threads.h"


static int MAX_LEN = 4096;


/*
  Used for passing multi args to pthread_create()
  Arguments: q1 - dequeue_from, q2 - enqueue_to
  Return Value: a pointer to a Multi_args struct
*/
Multi_args* CreateMultiArgs(void* q1, void* q2)
{
  Multi_args* ret = (Multi_args*)malloc(sizeof(Multi_args));
  handle_malloc_error(ret);

  ret->arg1 = (Queue*)q1; // q_from
  ret->arg2 = (Queue*)q2; // q_to

  return ret;
}


/*
  Reader: read from stdin, abort the lines over MAX_LEN, 
  replace '\n' with '\0', enqueue the valid lines
  Argument: pointer to the Queue to be enqueued
 */
void* func_reader(void* arg)
{
  Queue* q = (Queue*)arg;
  char ch = 'a'; // in order to enter the while loop
  int read_len = 0; // current available index
  char* buffer = (char*)malloc(MAX_LEN * sizeof(char));
  handle_malloc_error(buffer);
  
  while(ch != EOF){
    ch = fgetc(stdin);
    
    if(read_len == MAX_LEN){ // abort this line
      fprintf(stderr, "Input line too long.\n");
      
      while(1){ // keep reading the rest and abort them
	if(ch == '\n')
	  break;
	
	if(ch == EOF){ // over MAX_LEN and ends with EOF
	  free(buffer);
	  EnqueueString(q, NULL);
	  pthread_exit(NULL);
	}
	ch = fgetc(stdin);
      }
      
      read_len = 0; // reset the current available to 0
      continue;
    }
    
    if(ch != '\n' && ch != EOF){ // store into buffer
      buffer[read_len] = (char)ch;
      read_len++;
    }
    else{
      if(ch == EOF && read_len == 0){	
	EnqueueString(q, NULL);
	free(buffer);
	pthread_exit(NULL);	      
      }

      buffer[read_len] = '\0';
      
      // malloc'ed size is one more than required
      char* ret_str = malloc((read_len+2) * sizeof(char));
      handle_malloc_error(ret_str);
      
      // only copy the valid characters
      strncpy(ret_str, buffer, read_len+1);
      EnqueueString(q, ret_str);
      if(ch == '\n'){
	read_len = 0;
	continue;
      }
      else{
	break;
      }
    }
  }
  
  // reader finished, enqueue a null string
  EnqueueString(q, NULL);
  free(buffer);
  pthread_exit(NULL);
}


/*
  Munch1: scan the line, replace each space character with an 
  asterisk ("*") character.Then pass the line to thread Munch2
  through another queue of character strings.
  Arguments: a pointer to a Multi_args struct, with the 
  arg1 to be dequeue_from, arg2 to be enqueue_to
*/
void* func_munch1(void* args)
{
  Multi_args* x = (Multi_args*)args;
  // get the arguments
  Queue* q_from = x->arg1;
  Queue* q_to = x->arg2;

  while(1){
    char* str = DequeueString(q_from);
 
    // if string is NULL, then enqueue a NULL to q_to
    if(str == NULL){
      EnqueueString(q_to, NULL);
      break;
    }
    
    // otherwise, process the string
    for(int i = 0; i < (int)strnlen(str, MAX_LEN-1); i++){
      if(str[i] == ' '){
	str[i] = '*';
      }
    }
    // enqueue the string when finished
    EnqueueString(q_to, str);
  }
  pthread_exit(NULL);
}


/*
  Munch2: scan the line, convert lower case letters to upper case.
  Then pass the line to thread writer through another queue of 
  character strings.
  Arguments: a pointer to a Multi_args struct, with the 
  arg1 to be dequeue_from, arg2 to be enqueue_to  
*/
void* func_munch2(void* args)
{
  Multi_args* x = (Multi_args*)args;
  Queue* q_from = x->arg1;
  Queue* q_to = x->arg2;

  while(1){
    char* str = DequeueString(q_from);
    
    if(str == NULL){
      EnqueueString(q_to, NULL);
      break;
    }
    // process the string
    for(int i = 0; i < (int)strnlen(str, MAX_LEN-1); i++){
      if(islower(str[i])){
	str[i] = toupper(str[i]);
      }
    }
    // enqueue the string when finished
    EnqueueString(q_to, str);
  }
  pthread_exit(NULL);
}


/*
  Writer: dequeue from q and write the line to stdout.
  Argument: pointer to the Queue between munch2 and writer
*/
void* func_writer(void* q)
{
  Queue* x = (Queue*)q;
  int count = 0; // stores the # of outputs

  while(1){
    char* str = DequeueString(x);

    if(str == NULL)
      break;
    fprintf(stdout, "%s\n", str);
    count++;
    free(str);
  }
  // print to stdout
  fprintf(stdout,
	  "\nThe total number of strings processed to stdout is: %d\n\n",
	  count);
  
  pthread_exit(NULL);
}
