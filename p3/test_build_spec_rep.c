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
  check if the user input includes <
  input: string of user input, less_index, less_inside_index
  return: true if includes <, false otherwise
  note that less_index will be updated, less_inside_index may be updated
*/
bool has_less(int argc, char* argv[], int* less_index, int* less_inside_index)
{
  // traverse all words in user input
  for(int i = 0; i < argc; i++){
    int len = strlen(argv[i]);

    // traverse each char in each word
    for(int j = 0; j < len; j++){
      if(argv[i][j] == '<'){
	return true;
      }
      (*less_inside_index)++;
    }
    (*less_index)++;
  }
  return false;
}

/*
  check if the user input includes >
  input: string of user input, great_index, great_inside_index
  return: true if includes >, false otherwise
  note that great_index will be updated, great_inside_index may be updated
*/
bool has_great(int argc, char* argv[], int* great_index,
	       int* great_inside_index)
{
  for(int i = 0; i < argc; i++){
    int len = strlen(argv[i]);

    for(int j = 0; j < len; j++){
      if(argv[i][j] == '>'){
	return true;
      }
      (*great_inside_index)++;
    }
    (*great_index)++;
  }
  return false;
}

/*
  execute according to the value of target_name
  input: target_name can be either target or NULL
*/
void basic_exec_option(char* target_name,
		       bool redirect_flag, char* redirect_file_path)
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
	      redirect_flag, redirect_file_path);
  }
  else{
    // get the node according to target_name
    node* tmp = getNode(all_nodes_list, all_nodes_num, target_name);
    if(tmp == NULL){
      fprintf(stderr, "invalid target name: %s\n", target_name);
      exit(EXIT_FAILURE);
    }
    postorder(all_nodes_list, all_nodes_num, tmp,
	      redirect_flag, redirect_file_path);
  }
}


/*
  this function reads file_path and execute
  after execution, the last four params are updated for further use
  and there's no cycle if successfully executed
  input: file_path, pointer to target_nodes_num, pointer to all_nodes_num, 
         target_nodes_list, all_nodes_list
*/
void execute_by_fp(char* file_path,
		       int* target_nodes_num, int* all_nodes_num,
		       node** target_nodes_list, node** all_nodes_list){
  
  // need to check if file_path exists or not
  if(access(file_path, F_OK) == -1){
    fprintf(stderr, "%s not exist, exit...\n", file_path);
    exit(EXIT_FAILURE);
  }
  
  // now, we have to start parsing the file - file_path
  FILE* fp = fopen(file_path, "r");
  if(fp == NULL){
    fprintf(stderr, "%s fopen failed\n", file_path);
    exit(EXIT_FAILURE);
  }
  
  target_nodes_list = parsing(target_nodes_num, fp);
  
  all_nodes_list = get_all_nodes_list(target_nodes_list,
				      *target_nodes_num,
				      all_nodes_num);
  
  // check cycle for every node
  for(int i = 0; i < *all_nodes_num; i++){
    check_cycle(all_nodes_list[i], all_nodes_list, *all_nodes_num);
  }
  
}

// check existence of a file
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
  int less_index = 0; // index for <
  int less_inside_index = 0; // inside index for <
  int great_index = 0; // index for >
  int great_inside_index = 0; // inside index for >

  // flags for -f, < and >
  bool f_flag = has_f(argc, argv, &f_index);
  bool less_flag = has_less(argc, argv, &less_index, &less_inside_index);
  bool great_flag = has_great(argc, argv, &great_index, &great_inside_index);

  char* file_path = (char*)malloc(MAX_LEN_FILENAME*sizeof(char));
  handle_malloc_error(file_path);
    
  // if in basic_exec_option, i.e. three flags all false
  if(!f_flag && !less_flag && !great_flag){
    if(argc == 1){ // no target
      basic_exec_option(NULL, false, NULL);
    }
    else if(argc == 2){ // target specified
      basic_exec_option(argv[1], false, NULL);
    }
    else{
      fprintf(stderr, "invalid input\n");
      exit(EXIT_FAILURE);
    }
  }

  // for the case -f only
  else if(f_flag && !less_flag && !great_flag){
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
		    false, NULL);
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
	  
	  postorder(all_nodes_list, all_nodes_num, tmp, false, NULL);
	}
      else{
	fprintf(stderr, "invalid input\n");
	exit(EXIT_FAILURE);
      }
      	
      break;
	
      }
    } 
  }

  // for the case < only
  else if(!f_flag && less_flag && !great_flag){
    // now we have the less_index and less_inside_index

    // "./537make<file_path"
    if(argc == 1){
      // "./537make<" error
      if(less_inside_index == strnlen(argv[less_index], MAX_LEN_FILENAME) - 1){
	fprintf(stderr, "< needs an argument\n");
	exit(EXIT_FAILURE);
      }
      // "./537make<file_path"
      else{
	// the last/first index of file_path in argv[less_index]
	int fp_last_index = strnlen(argv[less_index], MAX_LEN_FILENAME) - 1;
	int fp_first_index = less_inside_index + 1;
	int fp_length = fp_last_index - fp_first_index + 1;
	
	for(int i = 0; i < fp_length; i++){
	  file_path[i] = argv[less_index][fp_first_index];
	  fp_first_index++;
	}
	file_path[fp_length] = '\0';
      }
    }

    else if(argc == 2){
      if(less_index == 0){ // "./537make< file_path"
	strncpy(file_path, argv[1], strlen(argv[1]));
      }
      else if(less_index == 1 && less_inside_index == 0){
	// "./537make <file_path"
	int fp_first_index = less_inside_index+1;
	int fp_length = strnlen(argv[1], MAX_LEN_FILENAME)-1;
	for(int i = 0; i < fp_length; i++){
	  file_path[i] = argv[less_index][fp_first_index];
	  fp_first_index++;
	}
	file_path[fp_length] = '\0';
      }
      else{
	fprintf(stderr, "invalid input\n");
	exit(EXIT_FAILURE);
      }
    }

    else if(argc == 3 && less_index == 1 && strlen(argv[1]) == 1){
      // "./537make < file_path"
      strncpy(file_path, argv[2], strlen(argv[2]));
    }

    else{
      fprintf(stderr, "invalid input\n");
      exit(EXIT_FAILURE);
    }

    // after we have file_path, execute.
    check_existence(file_path);
    
    FILE* fp = fopen(file_path, "r");
    if(fp == NULL){
      fprintf(stderr, "%s fopen failed\n", file_path);
      exit(EXIT_FAILURE);
    }
    else{
      fprintf(stderr, "invalid input\n");
      exit(EXIT_FAILURE);
    }
    
    int target_nodes_num;
    int all_nodes_num;
    
    node** target_nodes_list = parsing(&target_nodes_num, fp);
    
    node** all_nodes_list = get_all_nodes_list(target_nodes_list,
					       target_nodes_num,
					       &all_nodes_num);
    
    for(int i = 0; i < all_nodes_num; i++){
      check_cycle(all_nodes_list[i], all_nodes_list, all_nodes_num);
    }
    
    postorder(all_nodes_list, all_nodes_num, all_nodes_list[0],
	      false, NULL);
  }

  // for the case > only
  else if(!f_flag && !less_flag && great_flag){
    // now we have the great_index and great_inside_index

    // "./537make>file_path"
    if(argc == 1){
      // "./537make>" error
      if(great_inside_index == strnlen(argv[great_index],
				       MAX_LEN_FILENAME) - 1){
	fprintf(stderr, "> needs an argument\n");
	exit(EXIT_FAILURE);
      }
      // "./537make>file_path"
      else{
	// the last/first index of file_path in argv[great_index]
	int fp_last_index = strnlen(argv[great_index], MAX_LEN_FILENAME) - 1;
	int fp_first_index = great_inside_index + 1;
	int fp_length = fp_last_index - fp_first_index + 1;
	
	for(int i = 0; i < fp_length; i++){
	  file_path[i] = argv[great_index][fp_first_index];
	  fp_first_index++;
	}
	file_path[fp_length] = '\0';
      }
    }

    else if(argc == 2){
      if(great_index == 0){ // "./537make> file_path"
	strncpy(file_path, argv[1], strlen(argv[1]));
      }
      else if(great_index == 1 && great_inside_index == 0){
	// "./537make >file_path"
	int fp_first_index = great_inside_index+1;
	int fp_length = strnlen(argv[1], MAX_LEN_FILENAME)-1;
	for(int i = 0; i < fp_length; i++){
	  file_path[i] = argv[great_index][fp_first_index];
	  fp_first_index++;
	}
	file_path[fp_length] = '\0';
      }
      else{
	fprintf(stderr, "invalid input\n");
	exit(EXIT_FAILURE);
      }
    }

    else if(argc == 3 && great_index == 1 && strlen(argv[1]) == 1){
      // "./537make > file_path"
      strncpy(file_path, argv[2], strlen(argv[2]));
    }

    else{
      fprintf(stderr, "invalid input\n");
      exit(EXIT_FAILURE);
    }

    basic_exec_option(NULL, true, file_path);
        
  }

  else if(!f_flag && less_flag && great_flag){
    
  }

}