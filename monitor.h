#ifndef MONITOR_H
#define MONITOR_H

#include "shared.h"
#include <queue>
#include <pthread.h>

class Monitor { 
private:
  std::queue<bool> queue;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t unconsumed;
  pthread_cond_t availableSlots;
  int requestsInQueue;
  int capacity;

public:
  Monitor(int queueSize);
  void insert();
  void remove();


};

#endif
