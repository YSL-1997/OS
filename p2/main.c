/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "reader.c"
#include "munch1.c"
#include "munch2.c"
#include "writer.c"

void* func_reader(Queue* p);
  // this should be implemented in reader.c;

void* func_munch1(Queue* p);
  // this should be implemented in munch1.c

void* func_munch2(Queue* p);
  // this should be implemented in munch2.c

void* func_writer(Queue* p);
  // this should be implemented in writer.c

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
  
  // probably needs handle_pthread_error(int err), see concurr1.c
  pthread_create(&reader_thread, NULL, &func_reader, (void*)arg); // arg remains to be figured out
  pthread_create(&munch1_thread, NULL, &func_munch1, (void*)args);// args remains to be figured out
  pthread_create(&munch2_thread, NULL, &func_munch2, (void*)args);// args remains to be figured out
  pthread_create(&writer_thread, NULL, &func_writer, (void*)arg); // arg remains to be figured out

  // free the queues after using
  free(q_reader_munch1);
  free(q_munch1_munch2);
  free(q_munch2_writer);
  return 0;
}

void* func_reader(Queue* q){
  char* s = reader(.. parameters);
  EnqueueString(q, s);
}

void* func_munch1(Queue* q_from, Queue* q_to){
  char* s = munch1(q_from);// dequeue needs to be implemented in munch1()
  EnqueueString(q_to, s);
}

void* func_munch2(Queue* q_from, Queue* q_to){
  char* s = munch2(q_from); // dequeue needs to be implemented in munch2()
  EnqueueString(q_to, s);
}

void* func_writer(Queue* q){
  writer(q); // dequeue needs to be implemented in writer()
  
}

