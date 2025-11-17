#ifndef MONITOR_H
#define MONITOR_H

#include "seating.h"
#include <queue>
#include <pthread.h>
#include <semaphore.h>  

class Monitor { 
private:
  std::queue<RequestType> queue;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t unconsumed;
  pthread_cond_t availableSlots;
  pthread_cond_t availableVIP;


public:
  sem_t consumersCompleted;
  int requestsInQueue;
  int vipLimit;
  int totalAddedRequests;
  int maxRequests;
  int capacity;

  //logging
  unsigned int produced[RequestTypeN];
  unsigned int **consumed;
  unsigned int inRequestQueue[RequestTypeN];

  Monitor(int queueSize, int requestLimit, int vipLimit);
  void insert( RequestType );
  void remove( ConsumerType );

};

#endif
