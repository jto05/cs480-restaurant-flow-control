#include "producer.h"

#include <cstdio>
#include "seating.h"
#include <time.h>
#include <iostream>

Producer::Producer( Monitor *monitor,
    RequestType type,
    unsigned int sleepTime  ) {
  this->sleepTime = sleepTime;
  this->type = type;
  this->sleepTime = sleepTime; // sleepTime should be in milliseconds;
  this->monitor = monitor;

}

void Producer::start() {

  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000;
  SleepTime.tv_nsec = (sleepTime % 1000 ) * 1000000;

  bool completed = false;

  while ( !completed ) {

    // sleep to simulate time it takes to complete request
    nanosleep( &SleepTime, NULL );
    completed = monitor->insert( type );

  }

};

void* producer_start( void *ptr ) {

  Producer *p = ( Producer *) ptr;
  p->start();

}
