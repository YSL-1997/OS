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


#include "munch2.h"
#include <ctype.h>

void* func_munch2(void* args)
{
  Multi_args* x = (Multi_args*)args;
  
  Queue* q_from = x->arg1;
  Queue* q_to = x->arg2;

  while(1){
    char* str = DequeueString(q_from);
    if(str[0] == '\0'){
      break;
    }
    for(int i = 0; i < (int)strlen(str); i++){
      if(islower(str[i])){
	str[i] = toupper(str[i]);
      }
    }
    // TODO: manipulate the str
    EnqueueString(q_to, str);
  }

  pthread_exit(NULL);
}
