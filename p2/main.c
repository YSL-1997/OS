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

  // error handling implemented in CreateStringQueue()
  Queue* q_reader_munch1 = CreateStringQueue(size);
  Queue* q_munch1_munch2 = CreateStringQueue(size);
  Queue* q_munch2_writer = CreateStringQueue(size);
  
  // error handling implemented in CreteMultiArgs()
  Multi_args* munch1_args = CreateMultiArgs(q_reader_munch1,
					    q_munch1_munch2);
  Multi_args* munch2_args = CreateMultiArgs(q_munch1_munch2,
					    q_munch2_writer);
  
  // create threads, with error-handling
  handle_pthread_create_error(pthread_create(&reader_thread,
					     NULL,
					     &func_reader,
					     (void*)q_reader_munch1));

  handle_pthread_create_error(pthread_create(&munch1_thread,
					     NULL,
					     &func_munch1,
					     (void*)munch1_args));

  handle_pthread_create_error(pthread_create(&munch2_thread,
					     NULL,
					     &func_munch2,
					     (void*)munch2_args));

  handle_pthread_create_error(pthread_create(&writer_thread,
					     NULL,
					     &func_writer,
					     (void*)q_munch2_writer));
  
  // the main thread waits for all the 4 threads done their work
  handle_pthread_join_error(pthread_join(reader_thread, NULL));
  handle_pthread_join_error(pthread_join(munch1_thread, NULL));
  handle_pthread_join_error(pthread_join(munch2_thread, NULL));
  handle_pthread_join_error(pthread_join(writer_thread, NULL));
  
  // print the queues' status
  printf("Statistics for q_reader_munch1:\n");
  PrintQueueStats(q_reader_munch1);
  printf("Statistics for q_munch1_munch2:\n");
  PrintQueueStats(q_munch1_munch2);
  printf("Statistics for q_munch2_writer:\n");
  PrintQueueStats(q_munch2_writer);

  /*
    free() is not required in this case. Below is the order of free():
    free the strings that we've been processing in writer.c
    free the stringQueue in each queue
    free the statistics module
    free the args
    free the queues at last
  */
  
  return 0;
}
