#include "monitor.h"

#include <iostream>
#include "seating.h"
#include "log.h"

Monitor::Monitor(int queueSize, int maxRequests, int vipLimit) {
  this->capacity = queueSize;
  this->vipLimit = vipLimit;
  this->maxRequests = maxRequests;
  this->totalAddedRequests = 0;
  this->requestsInQueue = 0;

  //initialize consumed
  consumed = new unsigned int*[ConsumerTypeN];
  for ( int i = 0; i < RequestTypeN; i ++ ) {
    consumed[i] = new unsigned int[RequestTypeN];
  }

  pthread_cond_init( &unconsumed, NULL );
  pthread_cond_init( &availableSlots, NULL );
  pthread_cond_init( &availableVIP, NULL );
  sem_init( &consumersCompleted, 0, 0 );

};

void Monitor::insert( RequestType request ) {

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );


  //while queue is full
  while (requestsInQueue >= capacity) {
    pthread_cond_wait( &availableSlots, &lock );
  }

  if ( request == VIPRoom ) {
    while ( inRequestQueue[request] > 6 ) {
      pthread_cond_wait( &availableVIP, &lock );
    }
  } 
  
  queue.push( request );

  produced[request]++;
  inRequestQueue[request]++;

  requestsInQueue++;
  totalAddedRequests++;

  output_request_added(request, produced, inRequestQueue);

  pthread_cond_signal( &unconsumed );

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

void Monitor::remove( ConsumerType type ) {

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  // while queue is empty
  while (requestsInQueue <= 0) {
    pthread_cond_wait( &unconsumed, &lock );
  }

  RequestType r = queue.front();
  queue.pop();

  inRequestQueue[r]--;
  consumed[type][r]++;
  requestsInQueue--;

  output_request_removed(type, r, consumed[type], inRequestQueue);

  if ( r == VIPRoom ) 
    pthread_cond_signal( &availableVIP );
  

  pthread_cond_signal( &availableSlots );

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

