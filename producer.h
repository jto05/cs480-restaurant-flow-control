#ifndef PRODUCER_H
#define PRODUCER_H

#include <pthread.h>
#include <queue>

#include "monitor.h"
#include "robot.h"

using namespace std;

class Producer : public Robot{
private:
  int currRequests;
  static int maxRequests;
  static int requestsAdded;

public:
  Producer( Monitor*, unsigned int );
  void start();
  static init_shared_data( int );
};

#endif
