/*
 * Author: Julian To and Kaylee Muckerman
 * REDID: 130405272 and 130526510
 *
 * Entry point of dineseating program
 *
 */

#include <unistd.h>
#include <cstdio>
#include <pthread.h>

#include <iostream>

#include "log.h"
#include "producer.h"
#include "consumer.h"
#include "monitor.h"

/*
 * main( int, char** )
 *
 * @brief Main entry point of program that processes CLI arguments,
 *        creates Producer and Consumer objects, starts pthreads, and
 *        then waits the main thread until Consumer thread completion. 
 *
 * @param int the number of arguments
 * @param int the arguments given
 *
 * @return error code
 *
 */
int main( int argc, char** argv ) {
  
  using namespace std;

  // command line switch 
  int option;                      

  // initialize defautl values of argument parameters
  int requestLimit = 110;
  int maxQueueSize = 20;
  int maxVIPSeatings = 6;
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

    case 'r': // time that Rev-9 robot uses to process seating requests
      r_sleepTime = atoi(optarg);
      break;

    case 'g': // time the GeneralTable robot takes to insert requests
      g_sleepTime = atoi(optarg);
      break;

    case 'v': // time the VIPRoom robot takes to insert requests
      v_sleepTime = atoi(optarg);
      break;
    }
  }

  // declare pthread for each execution
  pthread_t gpThread;
  pthread_t vpThread;
  pthread_t xcThread;
  pthread_t rcThread;

  // initialize the Monitor, and each Consumer and Producer object using arguments
  Monitor *m = new Monitor( maxQueueSize, requestLimit, maxVIPSeatings );
  Producer *gp = new Producer( m, GeneralTable, (unsigned int) g_sleepTime );
  Producer *vp = new Producer( m, VIPRoom, (unsigned int) v_sleepTime );
  Consumer *xc = new Consumer( m, TX, (unsigned int) x_sleepTime );
  Consumer *rc = new Consumer( m, Rev9, (unsigned int) r_sleepTime );

  // start each thread
  pthread_create( &gpThread, NULL, &producer_start, gp );
  pthread_create( &vpThread, NULL, &producer_start, vp );
  pthread_create( &xcThread, NULL, &consumer_start, xc );
  pthread_create( &rcThread, NULL, &consumer_start, rc );

  // wait the main thread until BOTH consumers are completed
  sem_wait( &(m->consumersCompleted) );
  sem_wait( &(m->consumersCompleted) );

  // destroy consumers threads after completion (redudant)
  pthread_join( xcThread, NULL );
  pthread_join( rcThread, NULL );

  // final log report
  output_production_history( m->produced, m->consumed );
}
