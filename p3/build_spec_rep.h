#ifndef BUILD_SPEC_REP_
#define BUILD_SPEC_REP_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "process_exec.h"
#include "parsing.h"
#include "errorHandling.h"

/* return a list of all nodes in the graph */
node** get_all_nodes_list(node** target_nodes_list, int target_list_len,
			  int* all_nodes_num);

// check if the user input includes -f
bool has_f(int argc, char* argv[], int* f_index);

// execute according to the value of target_name
void basic_exec_option(char* target_name,
		       bool redirect_flag, char* redirect_input_file_path,
		       char* redirect_output_file_path);

void check_existence(char* file_path);

/*
  according to what is passed to the cmdline from the user
  determine the build specification
*/
void read_user_input(int argc, char** argv);

#endif
