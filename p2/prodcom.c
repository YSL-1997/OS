/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "threads.h"

int main(int argc, char* argv[]){
  pthread_t reader_thread;
  pthread_t munch1_thread;
  pthread_t munch2_thread;
  pthread_t writer_thread;
  
  int size = 10;
  // size of the shared queues
  
  Queue* q_reader_munch1 = CreateStringQueue(size);
  Queue* q_munch1_munch2 = CreateStringQueue(size);
  Queue* q_munch2_writer = CreateStringQueue(size);
  

  Multi_args* munch1_args = CreateMultiArgs(q_reader_munch1, q_munch1_munch2);
  Multi_args* munch2_args = CreateMultiArgs(q_munch1_munch2, q_munch2_writer);
  
  // ************************************************************
  // probably needs handle_pthread_error(int err), see concurr1.c
  
  pthread_create(&reader_thread, NULL, &func_reader, (void*)q_reader_munch1);
  printf("reader created... finished\n");
  pthread_create(&munch1_thread, NULL, &func_munch1, (void*)munch1_args);
  printf("munch1 created... finished\n");
  pthread_create(&munch2_thread, NULL, &func_munch2, (void*)munch2_args);
  printf("munch2 created... finished\n");
  pthread_create(&writer_thread, NULL, &func_writer, (void*)q_munch2_writer);
  printf("writer created... finished\n");
  
  // ************************************************************
  // probably needs handle_pthread_error(int err), see concurr1.c
  // ************************************************************

  // wait until the thread is done its work
  // so basically, the main thread is waiting for all the 4 threads done their
  // work
  // On success, pthread_join() returns 0; on error, it returns an error number.
  pthread_join(reader_thread, NULL);
  pthread_join(munch1_thread, NULL);
  pthread_join(munch2_thread, NULL);
  pthread_join(writer_thread, NULL);
  
  // print the queues' status
  PrintQueueStats(q_reader_munch1);
  PrintQueueStats(q_munch1_munch2);
  PrintQueueStats(q_munch2_writer);

  // free the strings that we've been processing in writer.c

  // free the stringQueue in each queue
  free(*(q_reader_munch1->stringQueue));  
  free(q_munch1_munch2->stringQueue);
  free(q_munch2_writer->stringQueue);
    
  free(munch1_args);
  free(munch2_args);
  
  // free the queues at last
  free(q_reader_munch1);
  free(q_munch1_munch2);
  free(q_munch2_writer);
  return 0;
}
