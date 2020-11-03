#ifndef BUILD_SPEC_REP_
#define BUILD_SPEC_REP_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


/* check if target_node needs to execute its cmdlines */
bool need_exec_cmd(node* target_node);

/*
  according to what is passed to the cmdline from the user
  determine the build specification
*/
void read_user_input(int argc, char* argv);

#endif