#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdbool.h>
#define TIMEQUANTUM 500
#define UNIT_TIME() { volatile unsigned long t; for(t=0;t<1000000UL;t++); }
enum Policy {
    FIFO = 0,
    SJF,
    PSJF,
    RR
};
int findMinExecTimeIndex(Process *schedProcess, int processNum);
void contextSwitch(Process *schedProcess, int from, int to);
void schedulingPreprocess(Process *schedProcess, int policy, int processNum);
void scheduling(Process *schedProcess, int policy, int processNum, bool isPreemptive);

#endif
