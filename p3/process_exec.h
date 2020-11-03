#ifndef EXECUTE_CMD_H_
#define EXECUTE_CMD_H_

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "graph.h"

// check if needs to execute cmd
bool need_exec_cmd(node* target_node);

// execute a cmd line
void execute_cmdline(int cmdWord_num, char** cmdWord);

// post-order traversal
void postorder(node** node_array, int all_nodes_num, node* root);

#endif
