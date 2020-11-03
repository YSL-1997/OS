#ifndef EXECUTE_CMD_H_
#define EXECUTE_CMD_H_

#include "errorHandling.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


// execute a cmd line
void execute_cmdline(int cmdWord_num, char** cmdWord);

// post-order traversal
void postorder(node** node_array, int all_nodes_num, node* root);

#endif
