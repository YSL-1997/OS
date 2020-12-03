/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "statistics.h"

statistics* stat_init(int num_process){
  statistics* ret = (statistics*)malloc(sizeof(statistics));
  ret->fake_AMU = 0;
  ret->fake_ARP = 0;
  ret->TMR = 0;
  ret->TPI = 0;
  ret->RTime = 0;
  ret->occupied_pages = 0;
  ret->runnable_proc = num_process;
  
  return ret;
}