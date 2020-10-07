/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

/*
  Munch1 will scan the line and replace each space character 
  (not tabs or newlines) with an asterisk ("*") character. 
  It will then pass the line to thread Munch2 through another 
  queue of character strings.
*/


// use index()

// 记得测试一下，假如，s是一个pointer，指向一个malloc'ed的string，然后修改s[0],s[1]...看是否在malloc'ed的string中也会被修改

void* func_munch1(void** args)
{
  Queue* q_from = (Queue*)args[0];
  Queue* q_to = (Queue*)args[1];

  char* str = DequeueString(q_from);
  // TODO: manipulate the str

  EnqueueString(q_to, str);
}
