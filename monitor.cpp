#include "monitor.h"

Monitor::Monitor(int queueSize) {
  this->capacity = queueSize;
  this->requestsInQueue = 0;
  pthread_cond_init( &unconsumed, NULL );
  pthread_cond_init( &availableSlots, NULL );
};

void Monitor::insert( RequestType request ) {

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  //while queue is full
  while (requestsInQueue >= capacity) {
    pthread_cond_wait( &unconsumed, &lock );
  }

  queue.push( request );
  requestsInQueue++;
  pthread_cond_signal( &unconsumed );

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

void Monitor::remove() {

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  // while queue is empty
  while (requestsInQueue <= 0) {
    pthread_cond_wait( &availableSlots, &lock );
  }

  queue.pop();
  requestsInQueue--;
  pthread_cond_signal( &availableSlots );

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

