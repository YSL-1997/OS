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
  execute a cmd line
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
  else if(pid == 0){ // child process created
    if(execvp(*cmd_arg, cmd_arg) < 0){
      fprintf(stderr, "cmd execution failed.\n");
      exit(1);
    }
  }
  else{ // this is the parent process
    while(wait(&status) != pid); // wait for child to complete
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
  printf("%s -> ", root->target);
  //if a target has cmd line, check the modification and then execute the cmd
  if(root->cmd_lines_num != 0){
    if(need_exec_cmd(root)){
      for(int i = 0; i < root->cmd_lines_num; i++){
        execute_cmdline(root->cmdArray[i]->cmdWord_num,
			root->cmdArray[i]->cmdWord);
      }
    }
  }
  // execute(root);
}
