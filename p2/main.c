/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "threads.h"

int main(){
  pthread_t reader_thread;
  pthread_t munch1_thread;
  pthread_t munch2_thread;
  pthread_t writer_thread;
  
  int size = 10;
  // size of the shared queues
  
  Queue* q_reader_munch1 = CreateStringQueue(size);
  Queue* q_munch1_munch2 = CreateStringQueue(size);
  Queue* q_munch2_writer = CreateStringQueue(size);
  // error handling implemented in CreateStringQueue()

  Multi_args* munch1_args = CreateMultiArgs(q_reader_munch1, q_munch1_munch2);
  Multi_args* munch2_args = CreateMultiArgs(q_munch1_munch2, q_munch2_writer);
  // error handling implemented in CreteMultiArgs()
  
  // create threads, with error-handling
  handle_pthread_create_error(pthread_create(&reader_thread, NULL,
				      &func_reader, (void*)q_reader_munch1));
  printf("reader created...\n");
  handle_pthread_create_error(pthread_create(&munch1_thread, NULL,
				      &func_munch1, (void*)munch1_args));
  printf("munch1 created...\n");
  handle_pthread_create_error(pthread_create(&munch2_thread, NULL,
				      &func_munch2, (void*)munch2_args));
  printf("munch2 created...\n");
  handle_pthread_create_error(pthread_create(&writer_thread, NULL,
				      &func_writer, (void*)q_munch2_writer));
  printf("writer created...\n");
  
  // the main thread waits for all the 4 threads done their work
  handle_pthread_join_error(pthread_join(reader_thread, NULL));
  handle_pthread_join_error(pthread_join(munch1_thread, NULL));
  handle_pthread_join_error(pthread_join(munch2_thread, NULL));
  handle_pthread_join_error(pthread_join(writer_thread, NULL));
  
  // print the queues' status
  PrintQueueStats(q_reader_munch1);
  PrintQueueStats(q_munch1_munch2);
  PrintQueueStats(q_munch2_writer);

  // free the strings that we've been processing in writer.c

  // free the stringQueue in each queue
  free(q_reader_munch1->stringQueue);  
  free(q_munch1_munch2->stringQueue);
  free(q_munch2_writer->stringQueue);

  // free the statistics module
  free(q_reader_munch1->statistics);
  free(q_munch1_munch2->statistics);
  free(q_munch2_writer->statistics);

  // free the args
  free(munch1_args);
  free(munch2_args);
  
  // free the queues at last
  free(q_reader_munch1);
  free(q_munch1_munch2);
  free(q_munch2_writer);
  
  return 0;
}
