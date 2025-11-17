#include "producer.h"

#include <cstdio>
#include "seating.h"
#include <time.h>
#include <iostream>

int Producer::requestsAdded;
int Producer::maxRequests;

Producer::Producer( Monitor *monitor,
    RequestType type,
    unsigned int sleepTime  ) : Robot(sleepTime) {
  this->requestType = type;
  this->sleepTime = sleepTime; // sleepTime should be in milliseconds;
  this->monitor = monitor;

}

void Producer::start() {

  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000;
  SleepTime.tv_nsec = (sleepTime % 1000 ) / 1000000;

  while ( monitor->totalAddedRequests < monitor->maxRequests ) {

    // sleep to simulate time it takes to complete request
    nanosleep( &SleepTime, NULL );
    monitor->insert( requestType );

  }

  std::cout << "finish" << std::endl;

};

void* producer_start( void *ptr ) {

  Producer *p = ( Producer *) ptr;
  p->start();

}
