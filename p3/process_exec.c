/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/


#include "process_exec.h"

/*
  This module is responsible for running each build command in a new process,
  waiting for its completion, and getting the return code.
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
    // need to check if the target already exists or not
    if(access(target_node->target, F_OK) == -1){
      // target not exist
      return true;
    }
    
    handle_stat_error(stat(target_node->target, &stat_list[0]));

    // the rest are dependencies info
    // need to check if the dependencies already exist or not
    for(int i = 0; i < target_node->dependency_num; i++){
      if(access(target_node->dependencies[i], F_OK) == -1){
	// dependency not exist
	return true;
      }
    }
    
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
    return false;
  }
}

/*
  execute one single cmd line
  input: num of words, words list
  note that: before calling this function, need to check stat()
*/
void execute_cmdline(int cmdWord_num, char** cmdWord)
{
  // malloc space for cmd arguments
  char** cmd_arg = (char**)malloc((cmdWord_num + 1) * sizeof(char*));
  handle_malloc_error(cmd_arg);

  // copy to cmd_arg, append NULL
  for(int i = 0; i < cmdWord_num; i++){
    cmd_arg[i] = cmdWord[i];
  }
  cmd_arg[cmdWord_num] = '\0';

  // prepare to fork()
  pid_t pid;
  int status;
  
  pid = fork();

  if(pid < 0){ // creation of a child process failed
    fprintf(stderr, "fork() failed to create a child process.\n");
    exit(1);
  }
  else if(pid == 0){ // we are now in the child process
    if(execvp(*cmd_arg, cmd_arg) == -1){
      fprintf(stderr, "cmd execution failed.\n");
      exit(EXIT_FAILURE);
    }
  }
  else{ // we are now in the parent process
    if(wait(&status) < 0){
	    perror("wait failed %d", status)
    	exit
		}
    else{
			if(WIFEXITED(status)){ // returns true if child process returns normally
				if(WEXITSTATUS(status)){// other errors (program returns nonzero)
					
				}
			}
			else{ // if child receives interrupt, seg fault
				error
			}
	}
	  //while(wait(&status) != pid); // wait for child to complete
  }
}

/* 
   post-order traversal
   input: all nodes, num of all nodes, a pointer to a root node
*/
void postorder(node** node_array, int all_nodes_num, node* root)
{

  for(int i = 0; i < root->dependency_num; i++){
    struct node* temp;
    temp = getNode(node_array, all_nodes_num, root->dependencies[i]);
    postorder(node_array, all_nodes_num, temp);
      
  }
  // printf("%s -> ", root->target);
  //if a target has cmd line, check the modification and then execute the cmd
  
  if(root->cmd_lines_num != 0){
    if(need_exec_cmd(root)){
      for(int i = 0; i < root->cmd_lines_num; i++){
	//	printf("execute: ");
	//for(int j = 0; j < root->cmdArray[i]->cmdWord_num; j++){
	//printf("%s ",root->cmdArray[i]->cmdWord[j]);
	//}
	//printf("\n");
        execute_cmdline(root->cmdArray[i]->cmdWord_num,
			root->cmdArray[i]->cmdWord);
      }
    }
  }
}
