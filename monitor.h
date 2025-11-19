/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
  * Header for Monitor class
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "seating.h"
#include <queue>
#include <pthread.h>
#include <semaphore.h>  

/*
 * Monitor class encapsulates critical region of 
 * consumer/producer interactions, facilitating
 * the modification of the request queue
 *
 * Also acts as a container for shared variables
 * used in both the Producer and Consumer class
 * methods
 *
 */
class Monitor { 
private:
  // request queue
  std::queue<RequestType> queue;

  // condition variables and mutex
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t unconsumed;
  pthread_cond_t availableSlots;
  pthread_cond_t availableVIP;


public:
  // shared semaphore for Consumer completion
  sem_t consumersCompleted; 

  int capacity;
  int vipLimit;
  int maxRequests;
  int totalAddedRequests;
  int requestsInQueue;

  //logging variables
  unsigned int produced[RequestTypeN];
  unsigned int **consumed;
  unsigned int inRequestQueue[RequestTypeN];

  /*
   * Monitor(int, int, int)
   *
   * @brief A constructor for a Consumer object
   *
   * @param capacity of queue (20 should always be passed in)
   * @param the maximum amount of requests to process 
   * @param the maximum amount of VIPs to seat ( should always be 6 )
   */
  Monitor(int queueSize, int requestLimit, int vipLimit);

  /*
   * insert(RequestType)
   *
   * @brief simulates insertion of request into queue in a
   *        mutually exclusive way, signaling to the consumers
   *        that there are requests to be consumed
   *
   * @param takes a RequestType to add into queue
   *
   * @return returns if max amount of requests have been reached
   */
  bool insert( RequestType );

  /*
   * remove(ConsumerType)
   *
   * @brief Removes from the queue in FIFO order, signaling to
   *        the producer that there is space in the queue 
   *
   * @param takes a ConsumerType for logging purposes
   *
   */
  void remove( ConsumerType );

};

#endif
