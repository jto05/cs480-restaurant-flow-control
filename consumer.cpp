#include "consumer.h"

#include <semaphore.h>
#include <time.h>
#include <iostream>

Consumer::Consumer( Monitor *m,
    ConsumerType type, 
    unsigned int sleepTime ) : Robot(sleepTime) {
  this->type = type;
  this->monitor = m;
}

void Consumer::start() {

  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000;
  SleepTime.tv_nsec = (sleepTime % 1000 ) * 1000000;

  while ( monitor->totalAddedRequests != monitor->maxRequests 
      || monitor->requestsInQueue != 0 ) {

    monitor->remove( type );
    nanosleep( &SleepTime, NULL );
  }

  sem_post( &(monitor->consumersCompleted) );
}

void *consumer_start( void* ptr ) {
  Consumer *c = ( Consumer* ) ptr;
  c->start();
}
