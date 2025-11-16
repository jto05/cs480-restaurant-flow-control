
#include <unistd.h>
#include <cstdio>
#include <pthread.h>

#include <iostream>

#include "producer.h"
#include "consumer.h"
#include "monitor.h"

int main( int argc, char** argv ) {
  
  using namespace std;

  int option;                      // command line switch 
  int idx;                         // general index 

  int requestLimit = 110;
  int maxQueueSize = 20;
  int x_sleepTime = 0;
  int r_sleepTime = 0;
  int g_sleepTime = 0;
  int v_sleepTime = 0;

  // Process command line arguments
  while ( (option = getopt(argc, argv, "s:x:r:g:v:")) != -1 ) {

    switch ( option )  {
    case 's': // total num of saeting requests for simulation
      requestLimit = atoi(optarg);
      break;

    case 'x': // time that T-X robut uses on average for processing seating requestk
      x_sleepTime = atoi(optarg);
      break;

    case 'r':  
      r_sleepTime = atoi(optarg);
      break;

    case 'g':
      g_sleepTime = atoi(optarg);
      break;

    case 'v':
      v_sleepTime = atoi(optarg);
      break;
    }
  }

  pthread_t gpThread;

  Monitor *m = new Monitor( maxQueueSize );
  Producer *gp = new Producer( m, (unsigned int) g_sleepTime );

  Producer::init_shared_data( requestLimit );

  pthread_create( &gpThread, NULL, &producer_start, gp );

  return 0;
}
