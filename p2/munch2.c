/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

/*
  Munch2 will scan the line and convert all lower case letters 
  to upper case (e.g., convert "a" to "A"). 
  It will then pass the line to thread Writer though yet another 
  queue of character strings.
*/

<<<<<<< HEAD
#include "munch2.h"

=======
#include "Queue.h"
#include <pthread.h>
>>>>>>> 9800f41770a07748f2d3b165513a4ff50adf8d0c

void* func_munch2(void** args)
{
  Queue* q_from = (Queue*)args[0];
  Queue* q_to = (Queue*)args[1];

  while(1){
    char* str = DequeueString(q_from);
    if(str[0] == '\0'){
      break;
    }
    // TODO: manipulate the str
    EnqueueString(q_to, str);
  }

  pthread_exit(NULL);
}
