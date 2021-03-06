/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/

#include "parsing.h"

static int MAX_LEN = 4096;
static int MAX_LEN_STR = 100;

/* 
   given a line string, split it into string array
   input: line string, line number, length of the array
   return: a pointer to a list of "words", i.e. the spilitted array
*/
char** split(char* string, int line_index, int* array_len)
{
  char ch = 'a'; //initialize character variable
  int col_num = 1000; // set the max length of string array
  int len = MAX_LEN_STR; //set the max length of each string
  int col_index = 0; //initialize the index of array
  int str_index = 0; //initialize the index of each string
  int read_len = 0; //initialize the index of line string
  int colon_flag = 0; //flag represent whether there is ':'
  int colon_error = 0; //flag represent whether colones are used correctly
  int i = 0;
  
  // create a pointer to string to store every parsing word
  char* str = (char*)malloc(len * sizeof(char));
  handle_malloc_error(str);
  
  char** string_arr = malloc(col_num * sizeof(char*));
  handle_malloc_error(string_arr);
  
  for(int j = 0; j < col_num; j++){
    string_arr[j] = malloc(len * sizeof(char)); 
    handle_malloc_error(string_arr[j]);
  }
  
  while(1){
    ch = string[i];
    
    if(ch == ' ' || ch == '\t' || ch =='\0'){
      // check whether the tab is the first character of a line. 
      // if yes, it is a command line and stored in the array.
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
      
      //if reach the end of line string, free memory and break
      if(ch == '\0'){
	free(str);
	break;
      }
      read_len++; 
    }
    
    else if(ch == ':'){
      // mark it as a line with :
      colon_flag = 1;
      
      // if the line start with target, no whitespace between target and colon
      if(col_index == 0 && str_index != 0){
	str[str_index] = '\0';
	strncpy(string_arr[col_index], str, str_index+1);
	col_index++;
	str_index = 0;
      }

      /*
	if the line start with target and
	there is whitespace between target and colon,
	then there's no error
      */
      else if(col_index == 1 && str_index == 0){}
      
      //if colon exists in dependency or commands, set the colon error flag
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
  
  *array_len = col_index; // update value of array_len

  if(string[0] == ' ' && !is_empty(string)){
    fprintf(stderr, "%d: <missing tab error>: \"%s\"\n",
	    line_index, string);
    exit(EXIT_FAILURE);
  }
  
  //print error message about colon usage error
  if(colon_error){
    fprintf(stderr, "%d: <colon usage error>: \"%s\"\n", line_index, string);
    exit(EXIT_FAILURE); 
  }
  
  //if a line is target+dependency but no colon included, print error message
  if(strcmp(string_arr[0],"\t") && strcmp(string_arr[0], "\0")
     && colon_flag == 0){
    fprintf(stderr, "%d: <colon usage error>: \"%s\"\n", line_index, string);
    exit(EXIT_FAILURE);
  }
  
  return string_arr;
}


/* 
   check if a line is blank
   input: a string
   return: true if is blank, false otherwise
*/
bool is_empty(char *s)
{
  // need to make sure that there
  while (*s != '\0') {
    if (!isspace(*s))
      return false;
    s++;    
  }
  
  return true;
}

/* 
   check for duplicate target or whether it is dependency only(not target)
   input: a node list, # of nodes, target string
   return: true if there's duplicate, false otherwise
*/
bool duplicate_target(node** node_array, int node_num, char* target)
{
  for(int i = 0; i < node_num; i++){

    if(!strcmp(node_array[i]->target, target)){
      return true;          
    }
  }
  return false;
}


/* 
   parsing the makefile line by line. If the line is not comment or 
   blank line, split it to string array. Distinguish whether the splited 
   line is target line or cmd. 

   if cmd, then store in cmd_node; 
   if target line, then store in node.

   input: a pointer to a var that stores the number of nodes 
   return: target node list, with nodes_num updated
*/ 
node** parsing(int* nodes_num, FILE* fp)
{
  // buffer used to store the line string
  char* buffer = (char*)malloc((MAX_LEN) * sizeof(char));
  handle_malloc_error(buffer);
  
  int line_index = 1;// index of line
  int read_len = 0;// index of character of each line
  int null_flag = 0; //flag represent whether there is '\0'
  char ch ='a';
  int word_num = 0; //# of strings for each line
  int nodes_num_init = 5; // initialize the length of node array
  node** node_array = malloc(nodes_num_init * sizeof(struct node*));
  handle_malloc_error(node_array);
  int node_index = 0;
  int cmdLine_num_init;//initialize # of cmd line for each target
  char** string_arr; //point to the splited string array
  int cmd_node_index = 0; //cmd index for each target
  cmdLine_num_init = 5;
    
  while(ch != EOF){
    ch = fgetc(fp);
    
    //if line length exceed the max length, report an error
    if(read_len == MAX_LEN){ 
      buffer[read_len-1] = '\0';
      fprintf(stderr, "%d: <Input line too long>: \"%s\"\n",
	      line_index, buffer);
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
	fprintf(stderr, "%d: <Line contains a null (zero) byte>: \"%s\"\n",
		line_index+1, buffer);
	exit(EXIT_FAILURE); 
      }

      // if the makefile is empty
      if(ch == EOF && read_len == 0 && line_index == 0){
	fprintf(stderr, "%d: <empty file>\n", 1);
	exit(EXIT_FAILURE); 
      }
      
      //if the line is blank or comment line, reset index then continue
      if( buffer[0] == '#' || is_empty(buffer)){
	if(ch == EOF){
	  node_array[node_index - 1] -> cmd_lines_num = cmd_node_index;
	  free(buffer);
	  break;	          
	}
	else{
	  read_len = 0;
	  line_index++;
	  continue;	          
	}	
      }
      
      string_arr = split(buffer, line_index, &word_num);
      
      //  if it's a (target + dependency) line, update the fields of node
      if(strcmp(string_arr[0], "\t")){
	
	//if the current node number reach the initial value, reallocate
	if(node_index == nodes_num_init){
	  int addition_nodes_num = 5;
	  // realloc more spaces
	  node_array = realloc(node_array,
			       (nodes_num_init + addition_nodes_num)
			       * sizeof(node*));
	  handle_malloc_error(node_array);
	  
	  nodes_num_init = nodes_num_init + addition_nodes_num;
	}

	// create node
	node_array[node_index] = CreateNode(word_num - 1, line_index,
					    cmdLine_num_init);
	strncpy(node_array[node_index]->target, string_arr[0], MAX_LEN_STR);
	strncpy(node_array[node_index]->target_line_string, buffer, MAX_LEN);
	
	//if no dependency, set dependency is null, else set the string
	// array slice(from index 1 to end) to dependency field
	if(word_num == 1){
	  free(node_array[node_index]->dependencies);
	  node_array[node_index]->dependencies = NULL;
	}
	else{
	  memcpy(node_array[node_index]->dependencies, &string_arr[1],
		 (word_num - 1)*sizeof(*string_arr));
	}
	
	//store the # of cmd line and check depulicate target
	if(node_index > 0){
	  node_array[node_index - 1]->cmd_lines_num = cmd_node_index;

	  // if there's duplicate target
	  if(duplicate_target(node_array, node_index - 1,
			     node_array[node_index]->target)){
	    fprintf(stderr, "%d: <Duplicate target>: \"%s\"\n",
		    line_index+1, buffer);
	    exit(EXIT_FAILURE); 
	  }
	}
	cmd_node_index = 0;
	node_index++;
      }
      
      // if it's a command line, update the fields of cmd_node
      else{
	
	//if the file start with cmd, report an error
	if(node_index == 0){
	  fprintf(stderr, "%d: <Command line with no target>: \"%s\"\n",
		  line_index+1, buffer);
	  exit(EXIT_FAILURE); 
	}
	
	else{
	  
	  //if the current # of cmd line reach the initial value, reallocate
	  if(cmd_node_index == cmdLine_num_init){
	    int addition_cmd_num = 5;
	    
	    node_array[node_index-1]->cmdArray =
	      realloc((node_array[node_index-1]->cmdArray),
		      (cmdLine_num_init+addition_cmd_num) * sizeof(cmd_node*));
	    
	    handle_malloc_error(node_array[node_index-1]->cmdArray);
	    cmdLine_num_init = cmdLine_num_init + addition_cmd_num;
	  }
	  
	  //if the cmd line splited string array is not only tab 
	  if(word_num > 1){
	    (node_array[node_index-1]->cmdArray)[cmd_node_index] =
	      CreateCmdNode(word_num - 1, line_index);
	    
	    memcpy((node_array[node_index-1]->
		    cmdArray)[cmd_node_index]->cmdWord,&string_arr[1],
		   (word_num - 1)*sizeof(*string_arr));
	    
	    strncpy((node_array[node_index-1]->
		     cmdArray)[cmd_node_index]->cmd_string, buffer, MAX_LEN);

	    cmd_node_index++;
	  }
	}
      }
      
      if(ch == '\n'){
	read_len = 0;
	line_index++;
      }
      else{
	// set the cmd line number for the last target
	node_array[node_index - 1] -> cmd_lines_num = cmd_node_index;
	free(buffer); 
      } 
    }
  }
  
  // update nodes_num
  *nodes_num = node_index;
  fclose(fp);

  return node_array;
}


/* the following functions are used for I/O redirection */

// convert char to string
char *strfromchar(char source)
{
  char* str = (char*)malloc(2 * sizeof(char));
  handle_malloc_error(str);
  str[0] = source;
  str[1] = '\0';
  
  return str;
}

/* the split_argv function are used to split the cmd which are associated, e.g.
   <input>output is splited into an array [<, input, > , output].
   Function arguments are # of words of command line, string array of command 
   line and total number of doubled splited string. 
*/
char** split_argv(int argc, char** argv, int* len_args_list)
{
  char ch = 'a'; //initialize character variable
  int col_num = 100; // set the max length of string array
  int len = 100; //set the max length of each string
  int word_index = 0; //initialize the index of array
  int str_index = 0; //initialize the index of each string
  int read_len = 0; //initialize the index of line string
  int less_num; // the number of '<'
  int greater_num; // the number of '>'
  
  // create a pointer to string to store every parsing word
  char* str = (char*)malloc(len * sizeof(char));
  handle_malloc_error(str);

  // create a pointer to string array to store totally splited argvs
  char** string_arr = malloc(col_num * sizeof(char*));
  handle_malloc_error(string_arr);

  for(int j = 0; j < col_num; j++){
    string_arr[j] = malloc(len * sizeof(char));
    handle_malloc_error(string_arr[j]);    
  }

  for(int i = 0; i < argc; i++){

    // if an argv includes "-f"
    if(strstr(argv[i], "-f") && argv[i][0] == '-'){
      // if the argv is not "-f", split argv into "-f" and filename
      if(strnlen(argv[i], 100) != 2){
	string_arr[word_index] = "-f";
	word_index++;
	string_arr[word_index] = argv[i] + 2;
	          
      }
      // if the argv is "-f", store it
      else{
	string_arr[word_index] = "-f";	          
      }
      word_index++;
    }

    // if an argv includes '<' or '>'
    else if(strchr(argv[i], '<') || strchr(argv[i], '>')){
      less_num = 0;
      greater_num = 0;
      
      // parsing the argv string
      while(1){
	ch = argv[i][read_len];
	
	if(ch == '<' || ch == '>'|| ch == '\0'){

	  if(ch == '<') less_num++;
	  if(ch == '>') greater_num++;
	  
	  // store the string before '<' or '>' if the
	  // previous char is not '<' or '>'
	  if(str_index != 0){
	    str[str_index] = '\0';
	    strncpy(string_arr[word_index], str, str_index+1);
	    word_index++;

	    // if the end of string, reset the index and break;
	    if(ch == '\0'){
	      read_len = 0;
	      str_index = 0;
	      break;                   
	    }
	    else{
	      strncpy(string_arr[word_index], strfromchar(ch), 4);
	      word_index++;
	      str_index = 0;	                          
	    }
	  }

	  else{
	    //if the end of string, break;
	    if(ch == '\0'){
	      read_len = 0;
	      break;
	    }
	    
	    //if the previous char is still '<' or '>', store the char as string
	    else{
	      strncpy(string_arr[word_index], strfromchar(ch), 4);
	      word_index++;
	      str_index = 0;
	    }
	  }
	}
	//if the current char is not '<' or '>', continue to parse
	else{
	  str[str_index] = (char) ch;
	  str_index++;
	}
	read_len++;
      }      
    }
    
    //if the argv doesn't include the above char, store it in the array
    else{
      strncpy(string_arr[word_index], argv[i], 100);
      word_index++;     
    }
  }
  
  *len_args_list = word_index; // update the length
  free(str);
  return string_arr;  
}


