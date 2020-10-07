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

void* func_munch2(void** args)
{
  Queue* q_from = (Queue*)args[0];
  Queue* q_to = (Queue*)args[1];

  char* str = DequeueString(q_from);
  // TODO: manipulate the str

  EnqueueString(q_to, str);
}
