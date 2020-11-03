#include "graph.h"
#include "parsing.h"

int main(int argc, char* argv[]){
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