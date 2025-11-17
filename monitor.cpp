#include "monitor.h"

#include <iostream>
#include "seating.h"
#include "log.h"

Monitor::Monitor(int queueSize, int maxRequests) {
  this->capacity = queueSize;
  this->maxRequests = maxRequests;
  this->addedRequests = 0;
  this->requestsInQueue = 0;
  pthread_cond_init( &unconsumed, NULL );
  pthread_cond_init( &availableSlots, NULL );
};

void Monitor::insert( RequestType request ) {

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  //while queue is full
  while (requestsInQueue >= capacity) {
    pthread_cond_wait( &availableSlots, &lock );
  }

  queue.push( request );

  produced[request]++;
  inRequestQueue[request]++;

  requestsInQueue++;
  addedRequests++;

  output_request_added(request, produced, inRequestQueue);

  pthread_cond_signal( &unconsumed );

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

void Monitor::remove() {

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  // while queue is empty
  while (requestsInQueue <= 0) {
    pthread_cond_wait( &unconsumed, &lock );
    
  }

  RequestType r = queue.front();
  queue.pop();
  inRequestQueue[r]--;
  requestsInQueue--;


  output_request_removed(TX, r, produced, inRequestQueue);


  pthread_cond_signal( &availableSlots );

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

