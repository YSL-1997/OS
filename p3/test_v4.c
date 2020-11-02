#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static int MAX_LEN = 4096;

/* construnct a struct to store info of cmd line */
typedef struct cmd_node
{
  int cmdWord_num; // # of splited words in cmd line
  int cmd_index; //the line index of a cmd line
  char* cmd_string; // the cmd line string
  char** cmdWord; //splited string array for a cmd line
}cmd_node;

/* construnct a struct to store info of cmd line */
typedef struct node
{
  char* target; // target string
  char** dependencies; //dependencies string array
  int dependency_num; //# of dependencies
  int target_line_index; // line index of target line
  char* target_line_string; //the target line string
  bool exploring; // the status of visited
  int cmd_lines_num; //# of cmd line for a target
  bool no_cycle; // cycle ststus of child node for parent
  cmd_node** cmdArray; // cmd node array
} node;

/* initialize a cmd node struct*/
cmd_node* CreateCmdNode(int cmd_size, int line_index){
  cmd_node* c = malloc(sizeof(struct cmd_node));
  c->cmdWord_num = cmd_size;
  c->cmd_index = line_index;
  c->cmdWord = (char**)malloc(sizeof(char*)*1000);
  // handle_malloc_error(n->dependencies); // error handling
  c->cmd_string = (char*) malloc(sizeof(char)* MAX_LEN);
  
  return c;
}

/* initialize a target node struct*/
node* CreateNode(int depen_size, int line_index, int cmdLine_num_init)
{
  
  node* n = (node*)malloc(sizeof(struct node));
  // handle_malloc_error(n); // error handling
  
  n->target = (char*) malloc(sizeof(char)*MAX_LEN);
  // handle_malloc_error(n->target); // error handling
  n->dependencies = (char**)malloc(sizeof(char*)*1000);
  // handle_malloc_error(n->dependencies); // error handling
  
  n->dependency_num = depen_size;
  n->target_line_index = line_index;
  
  n->target_line_string = (char*) malloc(sizeof(char)*MAX_LEN);
  // handle_malloc_error(n->line_string); // error handling
  n->exploring = false;
  n->no_cycle = false;
  n->cmd_lines_num = 0;
  n->cmdArray = malloc(cmdLine_num_init * sizeof(struct cmd_node*));
  return n;
}



/* given a line string, split it into string array*/
char** split(char* string, int line_index, int* array_len)
{
  char ch = 'a'; //initialize character variable
  int col_num = 1000; // set the max length of string array
  int len = MAX_LEN; //set the max length of each string
  int col_index = 0; //initialize the index of array
  int str_index = 0; //initialize the index of each string
  int read_len = 0; //initialize the index of line string
  int colon_flag = 0; //flag represent whether there is ':'
  int colon_error = 0; //flag represent whether colones are used correctly
  int i = 0;
  
  // create a pointer to str to store every parsing word
  char* str = (char*)malloc(len * sizeof(char));
  char** string_arr = malloc(col_num * sizeof(char*));
  // handle_malloc_error(string_arr);
  for(int j = 0; j < col_num; j++){
    string_arr[j] = malloc(len * sizeof(char)); 
    // handle_malloc_error( string_arr[j]);
  }
  while(1){
    ch = string[i];
    if(ch == ' ' || ch == '\t' || ch =='\0'){
      //check whether the tab is the first character of a line. 
      // If yes, it is a command line and stored in the array.
      if(ch == '\t' && col_index == 0 && read_len == 0){
	strncpy(string_arr[col_index], "\t", 10);
	col_index++;
        
      }
      
      /*
	if character is whitespace or tab(but not the first character),
	check the character before it. If the last character is not tab or
	whitespce, store the string into array.
	Then set the string index to zero and increase the column idex.
      */
      else if(str_index != 0){
	str[str_index] = '\0';
	strncpy(string_arr[col_index], str, str_index+1);
	col_index++;
	str_index = 0;
      } 
      
      //if reach the end of line string, free memory and break out
      if(ch == '\0'){
	free(str);
	break;
      }
      read_len++; 
    }
    
    else if(ch == ':'){
      // mark it as a line with :
      colon_flag = 1;
      // if the line start with target, no whitespace between target and colone
      if(col_index == 0 && str_index != 0){
	str[str_index] = '\0';
	strncpy(string_arr[col_index], str, str_index+1);
	col_index++;
	str_index = 0;
      }
      //if the line start with target, there is whitespace between target and colone
      else if(col_index == 1 && str_index == 0){}
      //if colone exists in dependency or commands, set the colon error flag
      else{
	colon_error = 1;
      }
      read_len++;
    }
    // store the string between two nearest space
    else{
      str[str_index] = (char) ch;
      str_index++;
      read_len++; 
    }
    
    i++;
  }
  
  *array_len = col_index;
  //print error message about colon usage error
  if(colon_error){
    fprintf(stderr, "%d: <colon usage error>: \"%s\"\n", line_index, string);
    exit(EXIT_FAILURE); 
  }
  
  //if a line is target+dependency but no colone included, print error message
  if(strcmp(string_arr[0],"\t") && strcmp(string_arr[0], "\0") && colon_flag == 0){
    fprintf(stderr, "%d: <colon usage error>: \"%s\"\n", line_index, string);
    exit(EXIT_FAILURE);
  }
  
  return string_arr;
}

/* check balnk line*/
bool isempty(char *s)
{
  while (*s != '\0') {
    if (!isspace(*s))
      return false;
    s++;
  }
  return true;
}

/* check duplicate target or whether it is only dependency but not target*/
bool duplicateTarget(node** node_array, int node_num, char* target){
  for(int i = 0; i < node_num; i++){
    if(!strcmp(node_array[i]->target, target)){
      return true;
    }
  }
  
  return false;
}

/* 
   parsing the makefile line by line. If the line is not comment or blank line,
   split it to string array. Distinguish whether the splited line is target line
   or cmd. If cmd, set fileds of cmd_node, otherwise, set fields of node.
*/ 
node** parsing(int* nodes_num)
{
  FILE *fp;
  
  // buffer used to store the line string
  char* buffer = (char*)malloc((MAX_LEN) * sizeof(char));
  // handle_malloc_error(buffer);
  
  int line_index = 0;// index of line
  int read_len = 0;// index of character of each line
  int null_flag = 0; //flag represent whether there is '\0'
  char ch ='a';
  int array_num = 0; //# of strings for each line
  int nodes_num_init = 5; // initialize the length of node array
  node** node_array = malloc(nodes_num_init * sizeof(struct node*));
  // handle_malloc_error( node_array);
  int node_index = 0;
  int cmdLine_num_init;//initialize # of cmd line for each target
  char** string_arr; //point to the splited string array
  int cmd_node_index = 0; //cmd index for each target

  fp = fopen("basic_cycle", "r");
  // handle_malloc_error(fp);
  
  while(ch != EOF){
    ch = fgetc(fp);
    //if line length exceed the max length, report an error
    if(read_len == MAX_LEN){ 
      buffer[read_len-1] = '\0';
      fprintf(stderr, "%d: <Input line too long>: \"%s\"\n", line_index, buffer);
      exit(EXIT_FAILURE);
    }
    //report error for '\0' character
    if(ch == '\0'){
      buffer[read_len] = (char)ch;
      read_len++;
      null_flag = 1;
    }
    // store the line string into buffer
    else if(ch != '\n' && ch != EOF){ 
      buffer[read_len] = (char)ch;
      read_len++;
    }
    
    else if(ch == '\n' || ch == EOF){ 
      buffer[read_len] = '\0';
      
      //print error message about null byte
      if(null_flag){
	fprintf(stderr, "%d: <Line contains a null (zero) byte>: \"%s\"\n", line_index+1, buffer);
	exit(EXIT_FAILURE); 
      }
      
      // if(ch == EOF && read_len == 0){
      //     fprintf(stderr, "%d: <empty file>\n", 0);
      //     exit(EXIT_FAILURE); 
      // }
      
      //if the line is blank or comment line, reset index then continue
      if(!strcmp(buffer, "\0") || buffer[0] == '#' || isempty(buffer)){
	read_len = 0;
	line_index++;
	continue;
      }
      
      string_arr = split(buffer, line_index, &array_num);
      // for(int i = 0; i<array_num; i++){
      
      //     printf("%s**", string_arr[i]);
      
      // }
      // if a command line, set cmd_node attributes
      if(strcmp(string_arr[0], "\t")){
	cmdLine_num_init = 5;
	//if the current node number reach the initial value, reallocate
	if(node_index == nodes_num_init){
	  printf("need realloc\n");
	  int addition_nodes_num = 5;
	  node_array = realloc(node_array,(nodes_num_init + addition_nodes_num) * sizeof(node*));
	  // handle_realloc_error(node_array[node_index-1] -> cmdArray);
	  nodes_num_init = nodes_num_init + addition_nodes_num;
	}
	
	node_array[node_index] = CreateNode(array_num - 1, line_index, cmdLine_num_init);
	strncpy(node_array[node_index] -> target, string_arr[0], MAX_LEN);
	strncpy(node_array[node_index] -> target_line_string, buffer, MAX_LEN);
	//if no dependency, set dependency is null, else set the string
	// array slice(from index 1 to end) to dependency field
	if(array_num == 1){
	  free(node_array[node_index] -> dependencies);
	  node_array[node_index] -> dependencies = NULL;
	}
	else{
	  memcpy(node_array[node_index] -> dependencies, &string_arr[1], (array_num - 1)*sizeof(*string_arr));
	}
        
	//     for(int i = 0; i<array_num-1; i++){
        
	//     printf("%s**", node_array[node_index] -> dependencies[i]);
        
	// }
	//store the # of cmd line and check depulicate target
	if(node_index > 0){
	  node_array[node_index - 1] -> cmd_lines_num = cmd_node_index;
	  // printf("cmd line number %d\n", node_array[node_index - 1] -> cmd_lines_num);
	  if(duplicateTarget(node_array, node_index - 1, node_array[node_index] -> target)){
	    fprintf(stderr, "%d: <Duplicate target>: \"%s\"\n", line_index+1, buffer);
	    exit(EXIT_FAILURE); 
	  }
	}
	cmd_node_index = 0;
	node_index++;
      }
      // if command line
      else{
	//if the file start with cmd, report an error
	if(node_index == 0){
	  fprintf(stderr, "%d: <Command line with no target>: \"%s\"\n", line_index+1, buffer);
	  exit(EXIT_FAILURE); 
	}
	else{
	  //if the current # of cmd line reach the initial value, reallocate
	  if(cmd_node_index == cmdLine_num_init){
	    int addition_cmd_num = 5;
	    node_array[node_index-1] -> cmdArray = realloc((node_array[node_index-1] -> cmdArray), \
							   (cmdLine_num_init+addition_cmd_num) * sizeof(cmd_node*));
	    // handle_realloc_error(node_array[node_index-1] -> cmdArray);
	    cmdLine_num_init = cmdLine_num_init + addition_cmd_num;
	  }
	  //if the cmd line splited string array is not only tab 
	  if(array_num > 1){
	    (node_array[node_index-1] -> cmdArray)[cmd_node_index] = CreateCmdNode(array_num - 1, line_index);
	    memcpy((node_array[node_index-1] -> cmdArray)[cmd_node_index] -> cmdWord, \
		   &string_arr[1], (array_num - 1)*sizeof(*string_arr));
	    strncpy((node_array[node_index-1] -> cmdArray)[cmd_node_index] -> cmd_string, buffer, MAX_LEN);
	    cmd_node_index++;
	  }
          
	}
        
      }
      
      if(ch == '\n'){
	read_len = 0;
	line_index++;
      }
      else{
	// set the cam line number for the last target
	node_array[node_index - 1] -> cmd_lines_num = cmd_node_index;
	free(buffer); 
      } 
      
      
    }
  }
  *nodes_num = node_index;
  fclose(fp);
  for(int i = 0; i< node_index; i++){
    printf("target: %s   ", node_array[i] -> target);
    for(int j = 0; j<node_array[i]->dependency_num; j++){
      printf(" dependencies: %s ", (node_array[i]->dependencies)[j]);
    }
    printf("\n");
    for(int k = 0; k < node_array[i] -> cmd_lines_num; k++){
      for(int m = 0; m<(node_array[i]->cmdArray)[k] -> cmdWord_num; m++){
	// printf("%s**", (node_array[i]->cmdArray)[k] -> cmd_string);
	printf("%s**", ((node_array[i]->cmdArray)[k] -> cmdWord)[m]);
      }
      printf("\n"); 
    }
  }
  // printf("susseee ");
  return node_array;
  
}



int in_nodes_list(char* dep_str, node** nodes_list, int nodes_list_len)
{
  for(int i = 0; i < nodes_list_len; i++){
    if(strncmp(dep_str, nodes_list[i]->target,
               strnlen(dep_str, MAX_LEN)) == 0){
      // meaning that to-be-added dependency already exists
      return 1;
    }
  }
  return 0;
}



node** get_all_nodes_list(node** target_nodes_list, int target_list_len, int* all_nodes_num)
{
  int num_nodes_init = target_list_len;
  // node_list is what is returned eventually, realloc() may be required
  node** nodes_list = (node**)malloc(sizeof(node*) * num_nodes_init);
  int num_nodes = 0; // stores the length of nodes_list
  
  // add the target_nodes_list to the node_list
  for(int i = 0; i < target_list_len; i++){
    nodes_list[i] = target_nodes_list[i];
    num_nodes++;
  }
  
  

  // next, need to add the dependencies to node_list
  // traverse target_nodes_list to get all dependencies
  for(int i = 0; i < target_list_len; i++){
    
    // for each target node, traverse all its dependencies
    for(int j = 0; j < target_nodes_list[i]->dependency_num; j++){
      
      // if the dependency is already in the nodes_list
      if(in_nodes_list(target_nodes_list[i]->dependencies[j],
                       nodes_list, num_nodes)){
        continue;
      }
      else{ // add to the node_list
	
	if(num_nodes == num_nodes_init){
	  int addition_num = 10;
	  nodes_list = realloc(nodes_list,(num_nodes_init + addition_num)*sizeof(node*));
	  // handle_realloc_error(n);
	  num_nodes_init = num_nodes_init + addition_num;
	}
        node* new_node = CreateNode(0, -1, 0);// new_node has no dependency
	
        // update the attribute - new_node->target
        strncpy(new_node->target, target_nodes_list[i]->dependencies[j],
                MAX_LEN);
	// strnlen(target_nodes_list[i]->dependencies[j], 100)-----------------------------------------------------------
	
        // set dependencies to be NULL
        new_node->dependencies = NULL;

        // set attribute - target_line_string to be NULL
        free(new_node->target_line_string);
        new_node->target_line_string = NULL;

        // set attribute - cmdArray to be NULL
        free(new_node->cmdArray);
        new_node->cmdArray = NULL;

        // add the new_node to node_list
        nodes_list[num_nodes] = new_node;
        num_nodes++;
      }
    }
  }
  *all_nodes_num = num_nodes;
  return nodes_list;
}



/*
  check_cycle() requires us to search for the node according to the
  'target' string given
  input: a string corresponds to a node's target, nodes_array, len(nodes_list)
  return value: a pointer to the corresponding node
*/
 
node* getNode(node** node_array, int all_nodes_num, char* target){
  for(int i = 0; i < all_nodes_num; i++){
    if(!strncmp(node_array[i] -> target, target, MAX_LEN)){
      return node_array[i];
    }
  }
  return NULL;
}

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
      fprintf(stderr, "%d: <cycle detected>: %s\n",
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
  Since we now have a list of all the nodes,
  need to check if there's a cycle for each node
*/

/* 
   reads the nodes in the nodes_list, check if there's a cycle in the
   dependency graph.
   if there's a cycle, set attribute - isCycle to be 1
   input: pointer to a node, all_nodes_list, len(all_nodes_list)
   return value: true if has cycle, false otherwise
   note that, after checking, the attribute - exploring will be set to false
*/


//check a string array whether contains a target
bool include(char** str_array, int str_num, char* str){
  for(int i = 0; i < str_num; i++){
    if(!strncmp(str_array[i], str, MAX_LEN)){
      return true;
    }
  }
  return false;
}

/*parsing the node array to find root nodes. If a node is a target but not a
  dependency, then it is a root node.
*/
char** getRoot(node** n, int t_num, int* root_num){
  bool flag = false; //root node flage
  int index = 0;
  int root_num_init = 10; 
  // mallocate memory for the array to store target and dependency string
  //需要realloc 吗
  char** root_array = (char**)malloc(sizeof(char*)*root_num_init);
  //  handle_malloc_error(targets_name)
  
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
      root_array[index] = (char*)malloc(sizeof(char)*MAX_LEN);
      //  handle_malloc_error(targets_name[i])
      strncpy(root_array[index], n[i]->target, MAX_LEN);
      index++;
    }
    flag = false;
  }
  *root_num = index; //set the total number of root node.
  return root_array;
}

// post-order traversal
void postorder(node** node_array, int all_nodes_num, node* root){
  
  for(int i = 0; i < root->dependency_num; i++){
    struct node* temp;
    temp = getNode(node_array, all_nodes_num, root->dependencies[i]);
    postorder(node_array, all_nodes_num, temp);
  }
  printf("%s -> ", root->target);
  // execute(root);
  
}



// void execute(node* root){

// }

int main(){
  node** node_array;
  int target_nodes_num;
  int all_nodes_num;
  int root_num;
  char** root_array;
  node_array = parsing(&target_nodes_num);
  // create_allnodes(node_array, target_nodes_num, &all_nodes_num);
  node** all_nodes_list = get_all_nodes_list(node_array, target_nodes_num, &all_nodes_num);
  printf("%s\n", all_nodes_list[0]->target);
  for(int i = 0; i < all_nodes_num; i++){
    printf("%s**", all_nodes_list[i]->target);
  }
  // CycleGraph = isCycleGraph(all_nodes_list, all_nodes_num);
  
  for(int i = 0; i < all_nodes_num; i++){
    check_cycle(all_nodes_list[i], all_nodes_list, all_nodes_num);
  }
  
  root_array = getRoot(node_array, target_nodes_num, &root_num);
  for(int i = 0; i < root_num; i++){
    printf("%s**", root_array[i]);
  }
  // for(int i = 0; i < root_num; i++){
  //     struct node* temp;
  //     temp = getNode(all_nodes_list, all_nodes_num, root_array[i]);
  //      printf("%d\n",  temp->dependency_num);
  //     postorder(all_nodes_list, all_nodes_num, temp);
  
  //     printf("\n");
  // }
  return 0;
}




