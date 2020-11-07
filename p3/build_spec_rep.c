/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/

#include "build_spec_rep.h"

/*
  This module contains the basic build specification abstraction.
  It allows you to create, update, and access a build specification.
*/

static int MAX_LEN_FILENAME = 4096;
static int MAX_LEN_STR = 4096;

/* 
   return a list of all nodes in the graph 
   with all_nodes_num updated
   input: target_nodes_list, len(target_nodes_list), pointer to an int
   return: a list of all nodes
*/
node** get_all_nodes_list(node** target_nodes_list, int target_list_len,
			  int* all_nodes_num)
{
  int num_nodes_init = target_list_len;
  int num_nodes = 0; // stores the length of nodes_list
  
  // node_list is what is returned eventually, realloc() may be required
  node** nodes_list = (node**)malloc(sizeof(node*) * num_nodes_init);
  handle_malloc_error(nodes_list);
  
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

	// if realloc is required
	if(num_nodes == num_nodes_init){
	  int addition_num = 10;
	  nodes_list = realloc(nodes_list,
			       (num_nodes_init + addition_num)*sizeof(node*));
	  handle_malloc_error(nodes_list);
	  num_nodes_init = num_nodes_init + addition_num; 
	}
	node* new_node = CreateNode(0, -1, 0);// new_node has no dependency

	// update the attribute - new_node->target
	strncpy(new_node->target, target_nodes_list[i]->dependencies[j],
		MAX_LEN_STR);


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
  check if the user input includes -f
  input: string of user input, f_index
  return: true if includes -f, false otherwise
  note that f_index will store the index of -f
*/
bool has_f(int argc, char* argv[], int* f_index)
{
  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "-f") == 0){
      return true;
    }
    (*f_index)++;
  }
  return false;
}

/*
  execute according to the value of target_name
  input: target_name can be either target or NULL,
         redirect_flag specifies whether redirect or not
	 redirect in and redirect out
*/
void basic_exec_option(char* target_name,
		       bool redirect_flag,
		       char* redirect_input_file_path,
		       char* redirect_output_file_path)
{
  int m_flag = 0; // flag for makefile
  int M_flag = 0; // flag for Makefile
  FILE* fp;
  
  if(access("makefile", F_OK) != -1) // makefile exists
    m_flag = 1;
  
  if(access("Makefile", F_OK) != -1) // Makefile exists
    M_flag = 1;

  // read makefile or Makefile 
  if(m_flag == 1)
    fp = fopen("makefile", "r");
  
  if(M_flag == 1 && m_flag == 0)
    fp = fopen("Makefile", "r");
  
  if(m_flag == 0 && M_flag == 0)
    fprintf(stderr, "No makefile or Makefile found\n");
  
  if(fp == NULL){
    fprintf(stderr, "fopen error\n");
    exit(EXIT_FAILURE);
  }
  
  int target_nodes_num; // # of target nodes
  int all_nodes_num; // # of all nodes

  // stores the target nodes
  node** target_nodes_list = parsing(&target_nodes_num, fp);

  // stores all nodes (targets + dependencies)
  node** all_nodes_list = get_all_nodes_list(target_nodes_list,
					     target_nodes_num,
					     &all_nodes_num);
  
  // check cycle for every node
  for(int i = 0; i < all_nodes_num; i++){
    check_cycle(all_nodes_list[i], all_nodes_list, all_nodes_num);
  }

  // specify root according to target_name
  if(target_name == NULL){
    postorder(all_nodes_list, all_nodes_num, all_nodes_list[0],
	      redirect_flag, redirect_input_file_path,
	      redirect_output_file_path);
  }
  else{
    // get the node according to target_name
    node* tmp = getNode(all_nodes_list, all_nodes_num, target_name);
    if(tmp == NULL){
      fprintf(stderr, "invalid target name: %s\n", target_name);
      exit(EXIT_FAILURE);
    }
    postorder(all_nodes_list, all_nodes_num, tmp,
	      redirect_flag, redirect_input_file_path,
	      redirect_input_file_path);
  }
}


/*
  check existence of a file
  input: file path
*/
void check_existence(char* file_path){
  if(access(file_path, F_OK) == -1){
    fprintf(stderr, "%s not exist, exit...\n", file_path);
    exit(EXIT_FAILURE);    
 }
}


/*
  according to what is passed to the cmdline from the user
  determine the build specification
  input: int argc, char* argv[]
*/
void read_user_input(int argc, char* argv[])
{
  int f_index = 0; // index for -f
  bool f_flag = has_f(argc, argv, &f_index);// flag for -f
 
  char* file_path = (char*)malloc(MAX_LEN_FILENAME*sizeof(char));
  handle_malloc_error(file_path);
    
  // if in basic_exec_option, i.e. three flags all false
  if(!f_flag){
    if(argc == 1){ // no target
      basic_exec_option(NULL, false, NULL, NULL);
    }
    else if(argc == 2){ // target specified
      basic_exec_option(argv[1], false, NULL, NULL);
    }
    else{
      fprintf(stderr, "invalid input\n");
      exit(EXIT_FAILURE);
    }
  }

  // for the case -f
  else{
    // if there's no argument after -f, that's an error.
    // 537make -f filename target AND/OR 537make target -f filename
    // 537make -f filename
    int ch;
    
    char* target_name = (char*)malloc(MAX_LEN_FILENAME*sizeof(char));
    handle_malloc_error(target_name);
    
    while((ch = getopt(argc, argv, "f:")) != -1){
      switch(ch){

      case 'f':
	// in this case, file_path has index f_index+1
	strncpy(file_path, optarg, MAX_LEN_FILENAME);

	// need to check if file_path exists or not
	check_existence(file_path);
       
	// now, we have to start parsing the file - file_path
	FILE* fp = fopen(file_path, "r");
	if(fp == NULL){
          fprintf(stderr, "%s fopen failed\n", file_path);
	  exit(EXIT_FAILURE);
	}

	 int target_nodes_num;
	 int all_nodes_num;
	 node** target_nodes_list = parsing(&target_nodes_num, fp);
	
	 node** all_nodes_list = get_all_nodes_list(target_nodes_list,
					    target_nodes_num,
					    &all_nodes_num);
  
	// check cycle for every node
	for(int i = 0; i < all_nodes_num; i++){
	  check_cycle(all_nodes_list[i], all_nodes_list, all_nodes_num);
	}
		
	// now, need to check if there's target specified
	if(argc == 3){
	  // no target, i.e. ./537make -f file_path
	  postorder(all_nodes_list, all_nodes_num, all_nodes_list[0],
		    false, NULL, NULL);
	}
	else if(argc == 4){
	  // there's target, need to find where the target is
	  int target_index = -1;

	  if(argc == f_index + 3){ // ./537make -f file_path target
	    target_index = f_index + 2;
	  }
	  else if(argc == f_index + 2){ // ./537make target -f file_path
	    target_index = f_index - 1;
	  }
	  else{
	    fprintf(stderr, "invalid input\n");
	    exit(EXIT_FAILURE);
	  }

	  // get target node
	  node* tmp = getNode(all_nodes_list,
			      all_nodes_num, argv[target_index]);
	  if(tmp == NULL){
	    fprintf(stderr, "invalid target name: %s\n", argv[target_index]);
	    exit(EXIT_FAILURE);
	  }
	  
	  postorder(all_nodes_list, all_nodes_num, tmp, false, NULL, NULL);
	}
      else{
	fprintf(stderr, "invalid input\n");
	exit(EXIT_FAILURE);
      }
      	
      break;
      }
    } 
  }
}
