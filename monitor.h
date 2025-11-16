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

public:
  int requestsInQueue;
  int capacity;

  Monitor(int queueSize);
  void insert( RequestType );
  void remove();

};

#endif
