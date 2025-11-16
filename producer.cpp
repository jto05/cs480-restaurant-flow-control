#include "producer.h"

#include <time.h>

Producer::Producer( Monitor *monitor,
    unsigned int sleepTime  ) : Robot(sleepTime) {
  this->sleepTime = sleepTime; // sleepTime should be in milliseconds;

}

void Producer::start() {

  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000000;
  SleepTime.tv_nsec = (sleepTime % 1000000 ) / 10000;


  while ( requestsAdded <= maxRequests ) {

    // sleep to simulate time it takes to complete request
    nanosleep( &SleepTime, NULL );
    monitor->insert( false );

  }

};



void Producer::init_shared_data(int n ) {
  this->maxRequests = n;
  this->requestsAdded = 0;
};
