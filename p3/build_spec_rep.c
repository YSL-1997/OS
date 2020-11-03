/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/

#include "build_spec_rep.h"
#include "graph.h"

/*
  This module contains the basic build specification abstraction.
  It allows you to create, update, and access a build specification.
*/


/*
  before executing the cmdline for each target,
  need to check if the target's dependencies are newer than target
  if(at least one of dependencies is newer than target), execute
  if(all dependencies is NOT newer than target), NOT execute
  input: a target node
  return: true if need to exec, false otherwise
*/
bool need_exec_cmd(node* target_node)
{
  
}





/*
  according to what is passed to the cmdline from the user
  determine the build specification
  input: int argc, char* argv[]
*/
void read_user_input(int argc, char* argv)
{
  int c;
  int 
  
  while((c = getopt(argc, argv, "f<>")) != -1){

    switch(c){

    case 'f':

      break;

    case '<':

      break;
    case '>':

      break;

    case '?':
      fprintf(stderr, "Unknown option -%c. \n", optopt);
      exit(1);
    }
  }
}
