#ifndef GRAPH_H_
#define GRAPH_H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "errorHandling.h"

/* a struct that stores info of each cmd line */
typedef struct cmd_node
{
  int cmdWord_num;
  // # of splited words in the cmd line

  int cmd_index;
  // the line index of the cmd line

  char* cmd_string;
  // the cmd line string

  char** cmdWord;
  // splited string array for the cmd line
  
}cmd_node;

/* construnct a struct to store info of target & dependency */
typedef struct node
{
  char* target;
  // target string

  char** dependencies;
  // string array of all dependencies

  int dependency_num;
  // # of dependencies for this target
  
  int target_line_index;
  // line number of the target line
  
  char* target_line_string;
  // the target line string
  
  bool exploring;
  // meaning that we have visited this node,
  // but not yet decided if it has a cycle
  
  int cmd_lines_num;
  // # of cmd line for a target
  
  bool no_cycle;
  // cycle ststus of child node for parent

  cmd_node** cmdArray;
  // array of pointers to cmd nodes
  
} node;


/* initialize a cmd node struct */
cmd_node* CreateCmdNode(int cmd_size, int line_index);

/* initialize a target node struct */
node* CreateNode(int depen_size, int line_index, int cmdLine_num_init);

/* check if a string name is in the nodes_list */
int in_nodes_list(char* dep_str, node** nodes_list, int nodes_list_len);

/* return a list of all nodes in the graph */
node** get_all_nodes_list(node** target_nodes_list, int target_list_len,
			  int* all_nodes_num);

/* get a pointer to the corresponding node */
node* getNode(node** node_array, int all_nodes_num, char* target);

/* check if there's a cycle for node t */
bool check_cycle(node* t, node** all_nodes_list, int all_nodes_list_len);

/* check if a string array contains a target */
bool include(char** str_array, int str_num, char* str);

/* get the root node list */
char** getRoot(node** n, int t_num, int* root_num);

#endif
