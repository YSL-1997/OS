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
  execute one single cmd line, with I/O redirection implemented
  input: num of words, words list, cmdNode, redirect_flag, write-to path
  note that: before calling this function, need to check stat()
  if you want to do the I/O redirection, set redirect_flag to be true
  and give the redirect_file_path
*/
void execute_cmdline(int cmdWord_num, char** cmdWord, cmd_node* cmdNode,
		     bool redirect_flag, char* redirect_input_file_path,
		     char* redirect_output_file_path)
{
  // malloc space for cmd arguments
  char** cmd_arg = (char**)malloc((cmdWord_num + 1) * sizeof(char*));
  handle_malloc_error(cmd_arg);

  // copy to cmd_arg, append NULL
  for(int i = 0; i < cmdWord_num; i++){
    cmd_arg[i] = cmdWord[i];
  }
  cmd_arg[cmdWord_num] = NULL;

  // prepare to fork()
  pid_t pid;
  int status;
  
  pid = fork();

  if(pid < 0){ // creation of a child process failed
    perror("fork() error: ");
    exit(EXIT_FAILURE);
  }
  
  else if(pid == 0){ // we are now in the child process

    // I/O redirection, only when redirect_flag == true
    if(redirect_flag){
      int redirect_in_fd = open(redirect_input_file_path, O_RDONLY);
      int redirect_out_fd = open(redirect_output_file_path,
				 O_CREAT | O_TRUNC | O_RDWR);
      handle_open_error(redirect_in_fd);
      handle_open_error(redirect_out_fd);
      
      // use dup2 to replace stdin & stdout with redirect_in/out_fd
      handle_dup2_error(dup2(redirect_in_fd, STDIN_FILENO));
      handle_dup2_error(dup2(redirect_out_fd, STDOUT_FILENO));
      
      handle_close_error(close(redirect_in_fd));
      handle_close_error(close(redirect_out_fd));
    }
    
    if(execvp(*cmd_arg, cmd_arg) == -1){
      // meaning that the cmd is not executable
      fprintf(stderr, "%d: <cmdline invalid>: \"%s\"\n",
	      cmdNode->cmd_index,
	      cmdNode->cmd_string);
      exit(EXIT_FAILURE);
    }
  }
  
  else{ // we are now in the parent process
    
    // parent calls wait, waiting for child to finish
    if(wait(&status) == -1){
      perror("wait() error: ");
      exit(EXIT_FAILURE);
    }
    else{
      // WIFEXITED returns true if the child terminated normally
      if(WIFEXITED(status)){
	// check the exit status of the child process
	if(WEXITSTATUS(status)){
	  perror("cmd cannot execute due to error");
	  exit(EXIT_FAILURE);
	}
      }
    }
  }
}

/* 
   post-order traversal
   input: all nodes, num of all nodes, a pointer to a root node
*/
void postorder(node** node_array, int all_nodes_num, node* root,
	       bool redirect_flag, char* redirect_input_file_path,
	       char* redirect_output_file_path)
{

  for(int i = 0; i < root->dependency_num; i++){
    struct node* temp;
    temp = getNode(node_array, all_nodes_num, root->dependencies[i]);
    postorder(node_array, all_nodes_num, temp,
	      redirect_flag, redirect_input_file_path,
	      redirect_output_file_path);
      
  }

  //if a target has cmd line, check the modification and then execute the cmd  
  if(root->cmd_lines_num != 0){
    if(need_exec_cmd(root)){
      
      for(int i = 0; i < root->cmd_lines_num; i++){
        execute_cmdline(root->cmdArray[i]->cmdWord_num,
			root->cmdArray[i]->cmdWord,
			root->cmdArray[i],
			redirect_flag,
			redirect_input_file_path,
			redirect_output_file_path);
      }
    }
  }
}
