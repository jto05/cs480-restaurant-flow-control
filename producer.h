#ifndef PRODUCER_H
#define PRODUCER_H

#include <pthread.h>
#include <queue>

#include "monitor.h"
#include "robot.h"

using namespace std;

class Producer {
private:
  unsigned int sleepTime;
  RequestType type;
  Monitor* monitor;

public:
  Producer( Monitor*, RequestType, unsigned int );
  void start();
};

void *producer_start( void* );

#endif
