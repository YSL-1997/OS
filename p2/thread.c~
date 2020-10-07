#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// This is an important file of thread implementations and definitions.

// From the man page, pthread_create() is as follows:
int pthread_create(pthread_t* thread, const pthread_attr_t* attr,
		   void *(*start_routine) (void *), void* arg);
/* 1st parameter: thread
   pointer to pthread_t
   
   2nd parameter: attr
   The  attr  argument points to a pthread_attr_t structure whose contents are
   used at thread creation time to determine attributes for  the  new  thread;
   this  structure is initialized using pthread_attr_init(3) and related functions.  
   If attr is NULL, then the thread is created with default attributes.

   3rd parameter: start_routine() - a pointer to a function
   The new thread starts execution by invoking start_routine() 

   4th parameter: arg
   arg is passed as the sole argument of start_routine().

   DESCRIPTION
       The  pthread_create()  function starts a new thread in the calling process.
       The new thread starts execution by invoking start_routine(); arg is  passed
       as the sole argument of start_routine().

       The new thread terminates in one of the following ways:

       * It  calls pthread_exit(3), specifying an exit status value that is avail‐
         able to another thread in the same process that calls pthread_join(3).

	 * It  returns  from  start_routine().   This  is  equivalent   to   calling
         pthread_exit(3) with the value supplied in the return statement.

	 * It is canceled (see pthread_cancel(3)).

	 * Any  of the threads in the process calls exit(3), or the main thread per‐
         forms a return from main().  This causes the termination of  all  threads
         in the process.

       The  attr  argument points to a pthread_attr_t structure whose contents are
       used at thread creation time to determine attributes for  the  new  thread;
       this  structure is initialized using pthread_attr_init(3) and related func‐
       tions.  If attr is NULL, then  the  thread  is  created  with  default  at‐
       tributes.

       Before  returning,  a  successful call to pthread_create() stores the ID of
       the new thread in the buffer pointed to by thread; this identifier is  used
       to refer to the thread in subsequent calls to other pthreads functions.

       The  new  thread  inherits  a  copy  of  the  creating thread's signal mask
       (pthread_sigmask(3)).  The set of pending signals for  the  new  thread  is
       empty  (sigpending(2)).   The  new  thread  does  not  inherit the creating
       thread's alternate signal stack (sigaltstack(2)).

       The new thread inherits the  calling  thread's  floating-point  environment
       (fenv(3)).

       The   initial   value  of  the  new  thread's  CPU-time  clock  is  0  (see
       pthread_getcpuclockid(3)).

   Linux-specific details
       The new thread inherits copies of the calling thread's capability sets (see
       capabilities(7)) and CPU affinity mask (see sched_setaffinity(2)).

RETURN VALUE
       On  success, pthread_create() returns 0; on error, it returns an error num‐
       ber, and the contents of *thread are undefined.

*/

// -------------------------------------------------------------------------------

int pthread_join(pthread_t thread, void** retval);
/*
  1st parameter: thread 
  represents the thread specified by "thread"

  2nd parameter: retval
  if retval is not set to NULL, 



  DESCRIPTION
       The  pthread_join()  function  waits  for the thread specified by thread to
       terminate.  If that thread has already terminated, then pthread_join()  re‐
       turns immediately.  The thread specified by thread must be joinable.

       If  retval  is  not NULL, then pthread_join() copies the exit status of the
       target  thread  (i.e.,  the  value  that  the  target  thread  supplied  to
       pthread_exit(3))  into  the  location  pointed to by retval.  If the target
       thread was canceled,  then  PTHREAD_CANCELED  is  placed  in  the  location
       pointed to by retval.

       If  multiple  threads  simultaneously try to join with the same thread, the
       results are undefined.  If the thread calling pthread_join()  is  canceled,
       then  the  target  thread  will  remain  joinable (i.e., it will not be de‐
       tached).

  RETURN VALUE
       On success, pthread_join() returns 0; on error, it returns an error number.
*/

// ----------------------------------------------------------------------------

// E.g. #1
void* hello(void *arg)
{
  printf("hello\n");
  return 0;
}

const int num_of_threads = 10;

int main()
{
  // here we create a list of threads - tid
  pthread_t tid[num_of_threads];

  for(int i = 0; i < num_of_threads; i++){
    pthread_create(&tid[i], NULL, hello, NULL);
    /*
      &tid[i] - the pointer to a thread
      NULL - default attribute
      hello - function pointer, points to function hello()
      NULL - arguments of hello(), is null
     */
  }

  for(int i = 0; i < num_of_threads; i++){
    pthread_join(tid[i], NULL);
  }
  return 0;
}

//---------------------------------------------------------------------

// E.g. #2
void* thread_func(void* p_arg)
{
  int* p = (int*)p_arg;
  int myNum = *p;
  printf("Thread number: %d\n", myNum);
  return 0;
}

int main()
{
  int i;
  pthread_t tid[num_of_threads];

  for(int i = 0; i < num_of_threads; i++){
    pthread_create(&tid[i], NULL, thread_func, &i);
    /*
      &i - 
     */
  }

  
}





// Thread function to generate sum of 0 to N

