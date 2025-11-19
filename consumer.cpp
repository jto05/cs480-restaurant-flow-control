/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
  * Source code for Consumer class
 */

#include "consumer.h"

#include <semaphore.h>
#include <time.h>
#include <iostream>

/*
 * Consumer(Monitor, ConsumerType, sleepTime)
 *
 * @brief A constructor for a Consumer object
 *
 * @param Monitor
 * @param ConsumerType; Rev-9 or T-X
 * @param unsigned int sleepTime
 */
Consumer::Consumer( Monitor *m,
    ConsumerType type, 
    unsigned int sleepTime ) {
  this->sleepTime = sleepTime;
  this->type = type;
  this->monitor = m;
}

/*
 * start()
 *
 * @brief Main loop of Consumer that consumes requests 
 *        inside of Monitor's queue
 *
 */
void Consumer::start() {

  // convert sleepTime into timespec to pass into nanosleep()
  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000;
  SleepTime.tv_nsec = (sleepTime % 1000 ) * 1000000;

  // keep consuming requests while theres still requests in the queue
  //    or the maximum amount of requests processed is reached
  while ( monitor->totalAddedRequests != monitor->maxRequests 
      || monitor->requestsInQueue != 0 ) {

    // access queue via monitor 
    monitor->remove( type ); 

    // sleep thread in order to simulate consumer robot time
    nanosleep( &SleepTime, NULL );

  }

  // post to semaphore inside of mainthread to signify a consumer is done
  sem_post( &(monitor->consumersCompleted) );
}


/*
 * consumer_start()
 *
 * @brief Entry point for Consumer threads; calls starT() 
 *        from Consumer object gathered from argument
 *
 * @param takes a void ptr that is cast to Consumer obj
 *
 */
void *consumer_start( void* ptr ) {
  Consumer *c = ( Consumer* ) ptr;
  c->start();
}
