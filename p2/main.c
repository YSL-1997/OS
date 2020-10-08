/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "reader.h"
#include "munch1.h"
#include "munch2.h"
#include "writer.h"

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
  
  Queue** munch1_args = {q_reader_munch1, q_munch1_munch2};
  Queue** munch2_args = {q_munch1_munch2, q_munch2_writer};
  // ************************************************************
  // probably needs handle_pthread_error(int err), see concurr1.c
  pthread_create(&reader_thread, NULL, &func_reader, (void*)q_reader_munch1);
  pthread_create(&munch1_thread, NULL, &func_munch1, (void**)munch1_args);
  pthread_create(&munch2_thread, NULL, &func_munch2, (void**)munch2_args);
  pthread_create(&writer_thread, NULL, &func_writer, (void*)q_munch1_munch2);

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
  PrintQueueStatus(q_reader_munch1);
  PrintQueueStatus(q_munch1_munch2);
  PrintQueueStatus(q_munch2_writer);

  // free the strings that we've been processing in writer.c

  // free the stringQueue in each queue
  free(q_reader_munch1->stringQueue);
  free(q_munch1_munch2->stringQueue);
  free(q_munch2_writer->stringQueue);

  // free the semaphores
  free(&q_reader_munch1->sem_en);
  free(&q_reader_munch1->sem_de);
  free(&q_reader_munch1->mutex);
  free(&q_munch1_munch2->sem_en);
  free(&q_munch1_munch2->sem_de);
  free(&q_munch1_munch2->mutex);
  free(&q_munch2_writer->sem_en);
  free(&q_munch2_writer->sem_de);
  free(&q_munch2_writer->mutex);

  
  // free the queues at last
  free(q_reader_munch1);
  free(q_munch1_munch2);
  free(q_munch2_writer);
  return 0;
}
