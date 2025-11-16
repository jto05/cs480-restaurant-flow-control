#include "producer.h"

#include "seating.h"
#include <time.h>

Producer::Producer( Monitor *monitor,
    unsigned int sleepTime  ) : Robot(sleepTime) {
  this->sleepTime = sleepTime; // sleepTime should be in milliseconds;
  this->monitor = monitor;

}

void Producer::start() {

  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000000;
  SleepTime.tv_nsec = (sleepTime % 1000000 ) / 10000;


  while ( requestsAdded <= maxRequests ) {

    // sleep to simulate time it takes to complete request
    nanosleep( &SleepTime, NULL );
    monitor->insert( GeneralTable );

  }

};



void Producer::init_shared_data(int n ) {
  maxRequests = n;
  requestsAdded = 0;
};
