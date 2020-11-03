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
  if(no dependency, but has cmdlines), execute
  input: a target node
  return: true if need to exec, false otherwise
*/
bool need_exec_cmd(node* target_node)
{
  if(target_node->dependencies != NULL){
    // compare last modification time of all dependencies with target

    // create a stat list that stores target+dependencies stat info
    struct stat* stat_list = malloc((target_node->dependency_num + 2)
				    * sizeof(struct stat));
    handle_malloc_error(stat_list);

    // the first is the target stat info
    handle_stat_error(stat(target_node->target, &stat_list[0]));

    // the rest are dependencies info
    for(int i = 0; i < target_node->dependency_num; i++){
      handle_stat_error(stat(target_node->dependencies[i], &stat_list[i+1]));
    }

    // compare stat_list[0] and stat_list[1:]
    for(int i = 1; i < target_node->dependency_num + 1; i++){
      if(stat_list[i].st_mtime > stat_list[0].st_mtime)
	return true;
    }

    return false;
  }
  
  else{ // means no dependency for this target
    // this corresponds to the case like: clean
    if(target_node->cmd_lines_num > 0){
      return true;
    }
  }
}





/*
  according to what is passed to the cmdline from the user
  determine the build specification
  input: int argc, char* argv[]
*/


// -f needs an argument. If there's no argument after -f, that's an error.''
// 537make -f filename target
void read_user_input(int argc, char* argv)
{
  
}
