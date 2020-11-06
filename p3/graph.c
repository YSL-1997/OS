/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/

#include "graph.h"



static int MAX_LEN = 4096;
static int MAX_LEN_STR = 4096;
static int MAX_NUM_DEPENDENCY = 1000;

/* 
   initialize a cmd node struct
   input: number of cmd, line_index of the cmd
   output: a pointer to the created cmd node
*/
cmd_node* CreateCmdNode(int cmd_size, int line_index)
{
  cmd_node* c = malloc(sizeof(struct cmd_node));
  handle_malloc_error(c); // error handling

  // update the attributes
  c->cmdWord_num = cmd_size;
  c->cmd_index = line_index;

  // malloc spaces for cmdWord and cmd_string
  c->cmdWord = (char**)malloc(sizeof(char*)*MAX_NUM_DEPENDENCY);
  handle_malloc_error(c->cmdWord);
  
  c->cmd_string = (char*) malloc(sizeof(char)* MAX_LEN);
  handle_malloc_error(c->cmd_string);
  
  return c;
  
}

/* 
   initialize a target node struct
   note that the node will be used in the graph traversal
   input: dependency number, line index, initialized cmdLine number
   output: a pointer to the created node
*/
node* CreateNode(int depen_size, int line_index, int cmdLine_num_init)
{
  // malloc spaces and errorHandling
  node* n = (node*)malloc(sizeof(struct node));
  handle_malloc_error(n);
  
  n->target = (char*) malloc(sizeof(char)*MAX_LEN_STR);
  handle_malloc_error(n->target);
  
  n->dependencies = (char**)malloc(sizeof(char*)*MAX_NUM_DEPENDENCY);
  handle_malloc_error(n->dependencies);

  n->target_line_string = (char*) malloc(sizeof(char)*MAX_LEN);
  handle_malloc_error(n->target_line_string);

  n->cmdArray = malloc(cmdLine_num_init * sizeof(struct cmd_node*));
  handle_malloc_error(n->cmdArray);
  
  // update the attributes according to the params
  n->dependency_num = depen_size;
  n->target_line_index = line_index;
  n->exploring = false;
  n->no_cycle = false;
  n->cmd_lines_num = 0;
  
  return n;
}

/* 
   check if a string name is in the nodes_list 
   input: string name, list of nodes, len(list of nodes)
   return: 1 if in the nodes_list, 0 otherwise.
*/
int in_nodes_list(char* dep_str, node** nodes_list, int nodes_list_len)
{
  for(int i = 0; i < nodes_list_len; i++){
    
    if(strncmp(dep_str, nodes_list[i]->target, MAX_LEN_STR) == 0){
      // meaning that to-be-added dependency already exists
      return 1;     
    }
  }
  return 0; 
}

/*
  check_cycle() requires us to search for the node according to the
  'target' string given.
  input: a string corresponds to a node's target, nodes_array, len(nodes_list)
  return: a pointer to the corresponding node
*/
node* getNode(node** node_array, int all_nodes_num, char* target)
{
  for(int i = 0; i < all_nodes_num; i++){

    if(!strncmp(node_array[i] -> target, target, MAX_LEN_STR)){
      return node_array[i];        
    }
  }
  return NULL;
}

/*
   reads the nodes in the all_nodes_list, check if there's a cycle in the
   dependency graph.
   if there's a cycle, set attribute - isCycle to be 1
   input: pointer to a node, all_nodes_list, len(all_nodes_list)
   return value: true if has cycle, false otherwise
   note that, after checking, the attribute - exploring will be set to false
*/
bool check_cycle(node* t, node** all_nodes_list, int all_nodes_list_len)
{
  // mark exploring of t to be true
  t->exploring = true;

  // check the attribute - no_cycle
  if(t->no_cycle){
    t->exploring = false;
    return false; // means no cycle
  }

  for(int i = 0; i < t->dependency_num; i++){

    // get the corresponding node according to dependencies[i]
    node* tmp = getNode(all_nodes_list, all_nodes_list_len, t->dependencies[i]);

    /*
      since the dependency-only (does not have dependencies) nodes
      cannot have cycle, we can conclude that if there's a cycle,
      then it must be a target_node (line number stored)
    */

    // check if the node is being explored, and no_cycle is false
    if(tmp->exploring == true && tmp->no_cycle == false){
      // means that there's a cycle when we met tmp
      tmp->no_cycle = false;
      fprintf(stderr, "%d: <cycle detected>: \"%s\"\n",
	      tmp->target_line_index, tmp->target_line_string);
      exit(1);
    }
    else{
      // recursion start, with tmp
      check_cycle(tmp, all_nodes_list, all_nodes_list_len);    
    }
  }

  // if we have traversed all the dependencies of t, with no cycle found
  // then for node t, there's no cycle
  t->no_cycle = true;
  t->exploring = false;

  return false;
}


/*
  check if a string array contains a target
  input: list of strings, # of strings, target string name
  return: true if yes, false otherwise
*/
bool include(char** str_array, int str_num, char* str)
{
  for(int i = 0; i < str_num; i++){
    if(!strncmp(str_array[i], str, MAX_LEN_STR)){
      return true;
    }      
  }
  return false;
}

/*
  parsing the node array to find root nodes. If a node is a target but not a
  dependency, then it is a root node.
  input: list of all nodes, num of all nodes, a pointer to an int that stores
  the # of root nodes
  return: a list of root nodes
*/
char** getRoot(node** n, int t_num, int* root_num)
{
  bool flag = false; //root node flage
  int index = 0;
  int root_num_init = 10;
  // mallocate memory for the array to store target and dependency string
  char** root_array = (char**)malloc(sizeof(char*)*root_num_init);
  handle_malloc_error(root_array);
  
  //comparing a target with all dependencies, if find, it's not root
  for(int i = 0; i < t_num; i++){
    for(int j = 0; j < t_num; j++){
      if(include(n[j]->dependencies, n[j]->dependency_num, n[i]->target)){
	flag = true;
	printf("include %s  ", n[i]->target);
	break;	
      }  
    }
    
    //if root number, store in an array
    if(!flag){
      root_array[index] = (char*)malloc(sizeof(char)*MAX_LEN_STR);
      //  handle_malloc_error(targets_name[i])
      strncpy(root_array[index], n[i]->target, MAX_LEN_STR);
      index++;
    }
    flag = false;
  }
  *root_num = index; //set the total number of root node.
  return root_array;  
}


