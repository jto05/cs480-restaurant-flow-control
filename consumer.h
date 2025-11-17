#ifndef CONSUMER_H
#define CONSUMER_H

#include "monitor.h"
#include "robot.h"

class Consumer : public Robot {
private:
  Monitor *monitor;
  ConsumerType type;

public:
  Consumer( Monitor*, ConsumerType, unsigned int );
  void start();
};
void *consumer_start( void* );

#endif
