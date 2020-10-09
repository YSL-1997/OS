/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "threads.h"


const int MAX_LEN = 100;


void* func_reader(void* arg){
  Queue* q = (Queue*)arg;
  FILE *fp;
  char ch = 'a'; // in order to enter the while loop
  int read_len = 0; // index of current length

  char* buffer = (char*)malloc(MAX_LEN * sizeof(char));

  if((fp = fopen("test3.txt", "r")) == NULL){
    perror("Could not get info from stdin");
    exit(EXIT_FAILURE);
      
  }

  while(ch != EOF){
    ch = fgetc(fp);
    // printf("Now we get a char: %c\n", ch);
    if(read_len == MAX_LEN){ // abort this line
      fprintf(stderr, "Input line too long.\n");
      // printf("%s \n", buffer);

      while(1){
	if(ch == '\n')
	  break;

	if(ch == EOF){
	  printf("The rest of the line ends with EOF, reader stop\n");
	  fclose(fp);
	  free(buffer);
	  EnqueueString(q, "");                                                 // modified (q, '\0')
	  printf("1_ reader: enqueued null\n");
	  pthread_exit(NULL);/////////////////////////////////////////////
	  
	}
	ch = fgetc(fp);
	      
      }

      read_len = 0; // reset i to be 0
      continue;
    }

    if(ch != '\n' && ch != EOF){
      buffer[read_len] = (char)ch;
      read_len++;
          
    }
    else{ // ch == '\n' or ch == EOF
      if(ch == EOF && q->enqueueCount == 0){
	printf("We have reached EOF\n");
	EnqueueString(q, "");                                                 // modified 本来是没有的
	printf("2_ reader: enqueued null\n");
	free(buffer);
	fclose(fp);
	pthread_exit(NULL);
	// have to check whether there will be multi-files in stdin,
	// if so, exit can't be used.
	      
      }

      buffer[read_len] = '\0';
      // now, we have that buffer[0 ~ read_len] is valid
      // need to put into a new malloc'ed str.
      if(read_len != 0){
	char* ret_str = malloc((read_len+1) * sizeof(char));
	strncpy(ret_str, buffer, read_len+1);
	// store what's in buffer to ret_str
	EnqueueString(q, ret_str);
	printf("3_ reader: Enqueued string: %s\n", ret_str);
      }
      
      if(ch == '\n'){
	read_len = 0;
	continue;
	      
      }
      else{ //ch == EOF
	break;
      }
    }
      
  }


  // read finished, enqueue a null char
  EnqueueString(q, "");                                                   // modified (q, str)
  printf("4_ reader: enqueued null\n");
  // not sure if this will work, if not, try replacing \0 with NULL.

  fclose(fp);
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
    if(str[0] == '\0'){
      printf("munch1 enqueued string: %s\n", str);
      EnqueueString(q_to, "");                                             // modified EnqueueString(q, str);
      
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
    if(str[0] == '\0'){
      printf("munch2 enqueued string: %s\n", str);
      EnqueueString(q_to, "");                                                 // modified EnqueueString(q, str);
      
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

  while(1){
    char* str = DequeueString(x);
    printf("writer dequeued string: %s\n", str);
    if(str[0] == '\0')
      break;
    printf("%s\n", str);
    free(str);
      
  }
  printf("writer ...........exit\n");
  pthread_exit(NULL);
  
}

Multi_args* CreateMultiArgs(void* q1, void* q2){
  Multi_args* ret = (Multi_args*)malloc(sizeof(Multi_args));
  ret->arg1 = (Queue*)q1;
  ret->arg2 = (Queue*)q2;
  return ret;
  
}
