/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
  * Header for Consumer class
 */

#ifndef CONSUMER_H
#define CONSUMER_H

#include "monitor.h"

/*
 * Consumer class that T-X and Rev-9 robot functions; encapsualates thread function of Consumers
 */
class Consumer {
private:
  unsigned int sleepTime;
  Monitor *monitor;
  ConsumerType type;

public:
  /*
   * Consumer(Monitor, ConsumerType, sleepTime)
   *
   * @brief A constructor for a Consumer object
   *
   * @param Monitor
   * @param ConsumerType; Rev-9 or T-X
   * @param unsigned int sleepTime
   */
  Consumer( Monitor*, ConsumerType, unsigned int );

  /*
   * start()
   *
   * @brief Main loop of Consumer that consumes requests 
   *        inside of Monitor's queue
   *
   */
  void start();
};

/*
 * consumer_start()
 *
 * @brief Entry point for Consumer threads; calls starT() 
 *        from Consumer object gathered from argument
 *
 * @param takes a void ptr that is cast to Consumer obj
 *
 */
void *consumer_start( void* );

#endif
