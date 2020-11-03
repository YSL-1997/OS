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

#endif