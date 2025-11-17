
#include <unistd.h>
#include <cstdio>
#include <pthread.h>

#include <iostream>

#include "log.h"
#include "producer.h"
#include "consumer.h"
#include "monitor.h"

int main( int argc, char** argv ) {
  
  using namespace std;

  int option;                      // command line switch 
  int idx;                         // general index 

  int requestLimit = 110;
  int maxQueueSize = 20;
  int maxVIPSeatings = 6;
  int x_sleepTime = 0;
  int r_sleepTime = 0;
  int g_sleepTime = 1;
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
  pthread_t vpThread;
  pthread_t xcThread;
  pthread_t rcThread;

  Monitor *m = new Monitor( maxQueueSize, requestLimit, maxVIPSeatings );
  Producer *gp = new Producer( m, GeneralTable, (unsigned int) g_sleepTime );
  Producer *vp = new Producer( m, VIPRoom, (unsigned int) v_sleepTime );
  Consumer *xc = new Consumer( m, TX, (unsigned int) x_sleepTime );
  Consumer *rc = new Consumer( m, Rev9, (unsigned int) r_sleepTime );

  pthread_create( &gpThread, NULL, &producer_start, gp );
  pthread_create( &vpThread, NULL, &producer_start, vp );
  pthread_create( &xcThread, NULL, &consumer_start, xc );
  pthread_create( &rcThread, NULL, &consumer_start, rc );

  sem_wait( &(m->consumersCompleted) );
  sem_wait( &(m->consumersCompleted) );

  pthread_join( xcThread, NULL );
  pthread_join( rcThread, NULL );

  output_production_history( m->produced, m->consumed );
}
