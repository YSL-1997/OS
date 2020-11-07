#ifndef PARSING_H_
#define PARSING_H_

#include "graph.h"

/* given a line string, split it into string array*/
char** split(char* string, int line_index, int* array_len);

/* check blank line*/
bool is_empty(char *s);

/* check duplicate target or whether it is only dependency but not target*/
bool duplicate_target(node** node_array, int node_num, char* target);

/*
   parsing the makefile line by line. If the line is not comment or blank line,
   split it to string array. Distinguish whether the splited line is target line
   or cmd. If cmd, set fileds of cmd_node, otherwise, set fields of node.
*/
node** parsing(int* nodes_num, FILE* path);

/* the following functions are used for I/O redirection */

/* convert char to string */
char *strfromchar(char source);

/* the split_argv function are used to split the cmd which are associated, e.g.
   <input>output is splited into an array [<, input, > , output].
   Function arguments are # of words of command line, string array of command
   line and total number of doubled splited string.
*/
char** split_argv(int argc, char** argv, int* len_args_list);

#endif