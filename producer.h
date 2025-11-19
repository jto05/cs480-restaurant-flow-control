/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
  * Header for Producer class
 */
#ifndef PRODUCER_H
#define PRODUCER_H

#include <pthread.h>
#include <queue>

#include "monitor.h"

/* 
 * The Producer class simulates an inidividual producer robot. This class
 * encapsulates Producer function and variables in order to simulate
 * the pushing of seating requests to our seating queue. 
 */
class Producer {
private:
  unsigned int sleepTime;
  RequestType type;
  Monitor* monitor;

public:
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
  Producer( Monitor*, RequestType, unsigned int );

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
  void start();
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
void *producer_start( void* );

#endif
