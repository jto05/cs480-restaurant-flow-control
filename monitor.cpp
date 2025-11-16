#include "monitor.h"

Monitor::Monitor(int queueSize) {
  this->maxQueueSize = queueSize;
  this->requestsInQueue = 0;
  pthread_cond_init( &unconsumed, NULL );
  pthread_cond_init( &availableSlots, NULL );
};

void Monitor::insert() {
  bool OnlyItem;

  pthread_mutex_lock( &lock );

  while (requestsInQueue >= maxQueueSize) {
    pthread_cond_wait( &unconsumed, &lock );
  }

  queue.push( true );
  pthread_cond_signal( &unconsumed );

  pthread_mutex_unlock( &lock );
}

