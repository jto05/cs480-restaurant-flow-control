/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
 * Source code for Producer class
 */

#include "producer.h"

#include <cstdio>
#include "seating.h"
#include <time.h>
#include <iostream>

/*
 * Producer( Monitor*, RequestType, unsigned int )
 *
 * @brief A constructor for the Producer class
 *
 * @param Monitor* a reference to the monitor containing shared variables
 * @param RequestType specifies prodcucer as VIPRoom or GeneralTable producer
 * @param unsigned int the sleepTime of the thread after producing
 *
 */
Producer::Producer( Monitor *monitor,
    RequestType type,
    unsigned int sleepTime  ) {
  this->type = type;
  this->sleepTime = sleepTime; // sleepTime should be in milliseconds;
  this->monitor = monitor;

}

/*
 * start()
 *
 * @brief The main function of Producer class. The method loops the insertion of
 *        requests until the maximum amount of requests is reached, sleeping at
 *        points in which queue is full, queue is being modified, or in which 
 *        there are no more VIP slots available
 *
 * @return void
 */
void Producer::start() {

  // Create time spec struct using sleepTime 
  struct timespec SleepTime;
  SleepTime.tv_sec = sleepTime / 1000;
  SleepTime.tv_nsec = (sleepTime % 1000 ) * 1000000;

  // while loop condition variable
  bool completed = false;

  while ( !completed ) {

    // sleep to simulate time it takes to complete request
    nanosleep( &SleepTime, NULL );
    completed = monitor->insert( type );

  }

};

/*
 * producer_start( void* )
 *
 * @brief The thread function of a Producer. It takes a Producer object and calls start()
 *        ending after start() finishes.
 *
 * @type Takes a void ptr that is cast to a Producer object in method
 *
 * @return void*
 */
void* producer_start( void *ptr ) {

  Producer *p = ( Producer *) ptr;
  p->start();

}
