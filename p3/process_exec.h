#ifndef EXECUTE_CMD_H_
#define EXECUTE_CMD_H_

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "graph.h"

// execute a cmd line
void execute_cmdline(int cmdWord_num, char** cmdWord);

// post-order traversal
void postorder(node** node_array, int all_nodes_num, node* root);

#endif
