#ifndef EXECUTE_CMD_H_
#define EXECUTE_CMD_H_

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "errorHandling.h"
#include "graph.h"
#include "parsing.h"

// check if needs to execute cmd
bool need_exec_cmd(node* target_node);

// execute a cmd line
// redirection implemented
void execute_cmdline(int cmdWord_num, char** cmdWord, cmd_node* cmdNode,
		     bool redirect_flag, char* redirect_input_file_path,
		     char* redirect_output_file_path);



// post-order traversal
void postorder(node** node_array, int all_nodes_num, node* root,
	       bool redirect_flag, char* redirect_input_file_path,
	       char* redirect_output_file_path);

#endif
