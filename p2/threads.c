/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "threads.h"


void* func_reader(void* arg){
  const int MAX_LEN = 4096;
  Queue* q = (Queue*)arg;
  char ch = 'a'; // in order to enter the while loop
  int read_len = 0; // current available index
  char* buffer = (char*)malloc(MAX_LEN * sizeof(char));
  handle_malloc_error(buffer); // error handling
  
  while(ch != EOF){
    ch = fgetc(stdin);
    if(ch != EOF)
      printf("ch = %c\n", ch);
    else
      printf("ch = EOF\n");
    if(read_len == MAX_LEN){ // abort this line
      fprintf(stderr, "Input line too long.\n");
      
      while(1){ // keep reading the rest and abort them
	if(ch == '\n')
	  break;
	
	if(ch == EOF){ // over MAX_LEN and ends with EOF
	  fprintf(stderr, "The rest of the line ends with EOF, reader stop\n");
	  free(buffer);
	  EnqueueString(q, NULL);
	  printf("1_ reader: enqueued null\n");
	  pthread_exit(NULL);
	}
	ch = fgetc(stdin);
      }
      
      read_len = 0; // reset the current available to 0
      continue;
    }// end if

    if(ch != '\n' && ch != EOF){ // store into buffer
      buffer[read_len] = (char)ch;
      read_len++;
    }
    else{ // ch == '\n' or ch == EOF
      if(ch == EOF && read_len == 0){	
	EnqueueString(q, NULL);
	printf("2_ reader: enqueued null\n");
	free(buffer);
	pthread_exit(NULL);	      
      }

      buffer[read_len] = '\0';
      // buffer[0 ~ read_len] is valid, store it.
      
      // malloc'ed size is one more than required
      char* ret_str = malloc((read_len+2) * sizeof(char));
      handle_malloc_error(ret_str); // error handling
      printf("************************inside buffer is : %s\n", buffer);
      // only copy the valid characters
      strncpy(ret_str, buffer, read_len+1);

      EnqueueString(q, ret_str);
      printf("3_ reader: Enqueued string: %s\n", ret_str);
  
      if(ch == '\n'){
	read_len = 0;
	continue;
      }
      else{ //ch == EOF && enqueueCount > 0
	break;
      }
    }
  }
  
  // read finished, enqueue a null string
  EnqueueString(q, NULL);
  printf("4_ reader: enqueued null\n");
  free(buffer);
  pthread_exit(NULL);
}


/*
  Munch1 will scan the line and replace each space character 
  (not tabs or newlines) with an asterisk ("*") character. 
  It will then pass the line to thread Munch2 through another 
  queue of character strings.
*/

// use index()


void* func_munch1(void* args)
{
  Multi_args* x = (Multi_args*)args;

  Queue* q_from = x->arg1;
  Queue* q_to = x->arg2;

  while(1){
    char* str = DequeueString(q_from);
    printf("munch1 dequeued string: %s\n", str);
    if(str == NULL){
      printf("munch1 enqueued string: %s\n", str);
      EnqueueString(q_to, NULL);
      break;
    }
    for(int i = 0; i < (int)strlen(str); i++){
      if(str[i] == ' '){
	str[i] = '*';
      }
    }

    EnqueueString(q_to, str);
    printf("munch1 enqueued string: %s\n", str);
  }
  printf("munch1 ...........exit\n");
  pthread_exit(NULL);
  // what passed inside the arg of pthread_exit() is returned by the function
}


void* func_munch2(void* args)
{
  Multi_args* x = (Multi_args*)args;

  Queue* q_from = x->arg1;
  Queue* q_to = x->arg2;

  while(1){
    char* str = DequeueString(q_from);
    printf("munch2 dequeued string: %s\n", str);
    if(str == NULL){
      printf("munch2 enqueued string: %s\n", str);
      EnqueueString(q_to, NULL);
      break;
    }
    for(int i = 0; i < (int)strlen(str); i++){
      if(islower(str[i])){
	str[i] = toupper(str[i]);
      }
    }
    EnqueueString(q_to, str);
    printf("munch2 enqueued string: %s\n", str);
  }
  printf("munch2 ...........exit\n");
  pthread_exit(NULL);
}



void* func_writer(void* q){
  Queue* x = (Queue*)q;
  int count = 0;
  while(1){
    char* str = DequeueString(x);
    printf("writer dequeued string: '%s' \n", str);
    if(str == NULL)
      break;
    printf("%s\n", str);
    count++;
    free(str);
  }
  printf("The total number of strings processed to stdout is: %d\n", count);
  printf("writer ...........exit\n");
  pthread_exit(NULL);
}

// Used for passing multi args to pthread_create() 
Multi_args* CreateMultiArgs(void* q1, void* q2){
  Multi_args* ret = (Multi_args*)malloc(sizeof(Multi_args));
  handle_malloc_error(ret); // error handling
  ret->arg1 = (Queue*)q1;
  ret->arg2 = (Queue*)q2;
  return ret;
}
