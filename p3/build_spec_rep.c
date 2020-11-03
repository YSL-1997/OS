/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/

#include "build_spec_rep.h"
#include "graph.h"
#include "parsing.h"

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
  check if the user input includes -f
  input: string of user input, f_index
  return: true if includes -f, false otherwise
  note that f_index will store the index of -f
*/
bool has_f(int argc, char** argv, int* f_index)
{
  for(int i = 0; i < argc, i++){
    if(strcmp(argv[i], "-f") == 0){
      return true;
    }
    *f_index++;
  }
}

/*
  check if the user input includes <
  input: string of user input, less_index, less_inside_index
  return: true if includes <, false otherwise
  note that less_index will be updated, less_inside_index may be updated
*/
bool has_less(int argc, char** argv, int* less_index, int* less_inside_index)
{
  // traverse all words in user input
  for(int i = 0; i < argc; i++){
    int len = strlen(argv[i]);

    // traverse each char in each word
    for(int j = 0; j < len; j++){
      if(argv[i][j] == '<'){
	return true;
      }
      *less_inside_index++;
    }
    *less_index++;
  }
}

/*
  check if the user input includes >
  input: string of user input, great_index, great_inside_index
  return: true if includes >, false otherwise
  note that great_index will be updated, great_inside_index may be updated
*/
bool has_great(int argc, char** argv, int* great_index, int* great_inside_index)
{
  for(int i = 0; i < argc; i++){
    int len = strlen(argv[i]);

    for(int j = 0; j < len; j++){
      if(argv[i][j] == '>'){
	return true;
      }
      *great_inside_index++;
    }
    *great_index++;
  }
}

/*
  check if the start of user input is correct
  only possible cases: 537make<*, 537make, 537make>*
  input: argv
  return: if not in the 3 possible cases, false; else true
*/
/*
bool start_correct(argv)
{
  char* str = "537make";
  int len = strlen(str);
  
  for(int i = 0; i < len; i++){
    if(argv[i] != str[i]){
      return false;
    }
  }

  // now, the first 7 chars of argv[0] is 537make
  // need to consider if argv[0] has other things
  if(strlen(argv[0]) > len){
    if(argv[0][len] == '<' || argv[0][len] == '>'){
      return true;
    }
    return false;
  }
  
}

*/

/*
  according to what is passed to the cmdline from the user
  determine the build specification
  input: int argc, char* argv[]
*/


// -f needs an argument. If there's no argument after -f, that's an error.''
// 537make -f filename target

/*
void read_user_input(int argc, char** argv)
{
  int f_index = 0; // index for -f
  int less_index = 0; // index for <
  int less_inside_index = 0; // inside index for <
  int great_index = 0; // index for >
  int great_inside_index = 0; // inside index for >

  // flags for -f, < and >
  bool f_flag = has_f(argc, argv, f_index);
  bool less_flag = has_less(argc, argv, &less_index, &less_inside_index);
  bool great_flag = has_great(argc, argv, &great_index, &great_inside_index);
  
  
  char* start_str = "537make";
  int start_str_len = strlen(start_str);

  // if the first word of user input is not 537make only
  if(strncmp(argv[0], start_str, strlen(argv[0])) != 0){

    // check if the first 7 chars are 537make
    // if not, then error
    for(int i = 0; i < start_str_len; i++){
      if(argv[0][i] != start_str[i]){
	fprintf(stderr, "Input should start with 537make\n");
	exit(EXIT_FAILURE);
      }
    }


    
  }
  else{ // the first word of user input is 537make

    // traverse argv, update the three flags
    
  }
}
*/



/*
  
*/
node** basic_exec_option(char* target_name)
{
  // read makefile or Makefile
  FILE* fp = fopen("makefile", "r");
  if(fp == NULL){
    fp = fopen("Makefile", "r");
    if(fp == NULL){
      fprintf(stderr, "No makefile or Makefile found\n");
      exit(1);
    }
  }
  
  int target_nodes_num;
  int all_nodes_num;
  
  node** target_nodes_list = parsing(&target_nodes_num, fp);
  
  node** all_nodes_list = get_all_nodes_list(target_nodes_list,
					     target_nodes_num,
					     &all_nodes_num);
  
  // check cycle for every node
  for(int i = 0; i < all_nodes_num; i++){
    check_cycle(all_nodes_list[i], all_nodes_list, all_nodes_num);
  }

  // specify root according to target_name
  if(target_name == NULL){
    postorder(all_nodes_list, all_nodes_num, all_nodes_list[0]);
  }
  else{
    // get the node according to target_name
    node* tmp = getNode(all_nodes_list, all_nodes_num, target_name);
    postorder(all_nodes_list, all_nodes_num, tmp);
  }
}


void read_user_input(int argc, char** argv)
{
  if(argc == 1 && strcmp(argv[0], "537make") == 0){
    // in terminal: 537make
    basic_exec_option(NULL);
  }
  else if(argc == 2 && strcmp(argv[0], "537make") == 0){
    // in terminal: 537make target_name
    basic_exec_option(argv[1]);
  }
}