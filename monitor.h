#ifndef MONITOR_H
#define MONITOR_H

#include "seating.h"
#include <queue>
#include <pthread.h>

class Monitor { 
private:
  std::queue<RequestType> queue;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t unconsumed;
  pthread_cond_t availableSlots;

  //logging
  unsigned int produced[RequestTypeN];
  unsigned int inRequestQueue[RequestTypeN];

public:
  int requestsInQueue;
  int addedRequests;
  int maxRequests;
  int capacity;

  Monitor(int queueSize, int requestLimit);
  void insert( RequestType );
  void remove();

};

#endif
