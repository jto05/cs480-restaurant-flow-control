/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
  * Source code for Monitor class
 */

#include "monitor.h"

#include <iostream>
#include "seating.h"
#include "log.h"

/*
 * Monitor(int, int, int)
 *
 * @brief A constructor for a Consumer object
 *
 * @param capacity of queue (20 should always be passed in)
 * @param the maximum amount of requests to process 
 * @param the maximum amount of VIPs to seat ( should always be 6 )
 */
Monitor::Monitor(int queueSize, int maxRequests, int vipLimit) {
  this->capacity = queueSize;
  this->vipLimit = vipLimit;
  this->maxRequests = maxRequests;
  this->totalAddedRequests = 0;
  this->requestsInQueue = 0;

  //initialize consumed logging values
  consumed = new unsigned int*[ConsumerTypeN];
  for ( int i = 0; i < RequestTypeN; i ++ ) {
    consumed[i] = new unsigned int[RequestTypeN]();
  }

  pthread_cond_init( &unconsumed, NULL );
  pthread_cond_init( &availableSlots, NULL );
  pthread_cond_init( &availableVIP, NULL );
  sem_init( &consumersCompleted, 0, 0 );

};

bool Monitor::insert( RequestType request ) {

  bool wasEmpty;

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  // unlock mutex and then return true that weve completed producing
  if ( totalAddedRequests >= maxRequests ) {
    // ensure consumers will not be waiting  
    pthread_cond_signal( &unconsumed );
    pthread_mutex_unlock( &lock );
    return true;
  }


  //while queue is full
  while (requestsInQueue >= capacity) {

    // wait thread until there are available slots in queue
    pthread_cond_wait( &availableSlots, &lock );

    // while waiting, we might hit maxRequests so we still return true here
    if ( totalAddedRequests >= maxRequests ) {
      // ensure consumers will not be waiting  
      pthread_cond_signal( &unconsumed );
      pthread_mutex_unlock( &lock );
      return true;
    }
  }

  if ( request == VIPRoom ) {

    while ( inRequestQueue[request] >= vipLimit ) {

      // wait thread until there are available VIP slots in queue
      pthread_cond_wait( &availableVIP, &lock );

      // while waiting, we might hit maxRequests so we still return true here
      if ( totalAddedRequests >= maxRequests ) {
        pthread_cond_signal( &unconsumed );
        pthread_mutex_unlock( &lock );
        return true;
      }
    }
  } 
  
  wasEmpty = queue.empty();


  // push request to back of queue
  queue.push( request );



  // logging values
  produced[request]++;
  inRequestQueue[request]++;
  requestsInQueue++;
  totalAddedRequests++;

  // logging function
  output_request_added(request, produced, inRequestQueue);

  // only signal to consumers if our queue was initially empty
  if ( wasEmpty ) {
    pthread_cond_signal( &unconsumed );
  }


  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
  
  // unless we have reached max requests, return false that we are NOT done
  return false;
}


/*
 * remove(ConsumerType)
 *
 * @brief Removes from the queue in FIFO order, signaling to
 *        the producer that there is space in the queue 
 *
 * @param takes a ConsumerType for logging purposes
 *
 */
void Monitor::remove( ConsumerType type ) {

  bool atCapacity;

      // * BEGIN CRITICAL REGION *
  pthread_mutex_lock( &lock );

  // while queue is empty
  while (requestsInQueue <= 0) {

    // a hard exit out of loop if maxRequests is far
    if (totalAddedRequests >= maxRequests) { 
     pthread_mutex_unlock(&lock);
     return;  
    }

    // wait consumer until signaled by producer that there are 
    //    requests to consume
    pthread_cond_wait( &unconsumed, &lock );
  }

  atCapacity = ( requestsInQueue >= capacity );

  // get request from front of queue 
  RequestType r = queue.front();
  queue.pop();

  // logging values
  inRequestQueue[r]--;
  consumed[type][r]++;
  requestsInQueue--;

  // logging function
  output_request_removed(type, r, consumed[type], inRequestQueue);

  if ( atCapacity ) {
    // signal to the producer that there are slots available
    //    if it WAS at capacity
    pthread_cond_signal( &availableSlots );

    if ( r == VIPRoom )  {
      // signal to the producer that there are VIP slots available
      //    if it WAS at capacity
      pthread_cond_signal( &availableVIP );
    }
  }

  pthread_mutex_unlock( &lock );
      // * END CRITICAL REGION *
}

