/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#ifndef statistics_H_
#define statistics_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// create a struct of statistics
typedef struct statistics{

  // Average Memory Utilization 
  double fake_AMU;

  // Average Runable Processes
  double fake_ARP;

  // Total Memory References
  unsigned long TMR;

  // Total Page Ins
  unsigned long TPI;

  // Running Time
  unsigned long RTime;

  // # of page frames occupied
  unsigned long occupied_pages;

  // # of running process
  unsigned long runnable_proc;

}statistics;

statistics* stat_init();

#endif