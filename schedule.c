#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "process.h"
#include "schedule.h"
#include "cpustate.h"

int processCmp(const void *p1, const void*p2) {
    Process *process1 = (Process *)p1;
    Process *process2 = (Process *)p2;
    if (process1->readyTime < process2->readyTime)
        return -1;
    else if (process1->readyTime > process2->readyTime)
        return 1;
    else {
        if (process1->index < process2->index)
            return -1;
        else
            return 1;
    }
}
int findMinExecTimeIndex(Process *schedProcess, int processNum) {
    int currentMinExecTime = INT32_MAX, switchToIndex = -1;
    for (int i = 0; i < processNum; i++) {
        if (schedProcess[i].state == ready && schedProcess[i].execTime < currentMinExecTime) {
            switchToIndex = i;
            currentMinExecTime = schedProcess[i].execTime;
        }
    }
    return switchToIndex;
}
void contextSwitch(Process *schedProcess, int from, int to) {
    // raise the priority of swithToIndex if it is not -1
    if (to != -1)
        raiseProcessPriority(schedProcess[to].pid);
    // lower the priority of runningIndex if it is not -1
    if (from != -1)
        lowerProcessPriority(schedProcess[from].pid);
}
void schedulingPreprocess(Process *schedProcess, int policy, int processNum) {
    qsort(schedProcess, processNum, sizeof(Process), processCmp);
    bool isPreemptive;
    if (policy == FIFO || policy == SJF) 
        isPreemptive = false;
    else if (policy == PSJF || policy == RR)
        isPreemptive = true;
    scheduling(schedProcess, policy, processNum, isPreemptive);
}
void scheduling(Process *schedProcess, int policy, int processNum, bool isPreemptive) {
    int readyNum = 0, finishedNum = 0, currentTime = 0;
    int runningIndex = -1;
    int RRCount = 0;

    assignProcessToOneCpu(getpid(), scheduleCPU);
    raiseProcessPriority(getpid());

    int queueLen = 0;
    int *readyQueue = (int *)malloc(sizeof(int)*(processNum+1)); 
    while (finishedNum < processNum) {
        /* check if there is any process ready */
        bool newlyAddProcess = false;
        int startIndex = readyNum;
        for (int i = startIndex; i < processNum; i++) {
            if (schedProcess[i].readyTime == currentTime) {
                schedProcess[i].pid = execProcess(schedProcess[i]);
                schedProcess[i].state = ready;
                readyNum = i+1;
                newlyAddProcess = true;
                readyQueue[queueLen] = i;
                queueLen++;
            }
            else
                break;
        }
        /* select the running process */
        if (policy == SJF || policy == PSJF) {
            /* check if there is a newly added process whose execTime is less than currently running process */
            if (isPreemptive) {
                if (newlyAddProcess || runningIndex == -1) {
                    int switchToIndex = findMinExecTimeIndex(schedProcess, processNum);
                    if (switchToIndex != runningIndex)
                        contextSwitch(schedProcess, runningIndex, switchToIndex);
                    runningIndex = switchToIndex;
                }
            }
            else
                if (runningIndex == -1) {
                    runningIndex = findMinExecTimeIndex(schedProcess, processNum);
                    if (runningIndex != -1)
                        raiseProcessPriority(schedProcess[runningIndex].pid);                
                }
        }
        else {
            if (runningIndex == -1) {
                if (queueLen > 0) {
                    runningIndex = readyQueue[0];
                    raiseProcessPriority(schedProcess[runningIndex].pid);
                }
            }
        }
        /* delay for one unit time */
        UNIT_TIME();
        currentTime++;
        if (runningIndex != -1) {
            RRCount++;
            schedProcess[runningIndex].execTime--;
            if (schedProcess[runningIndex].execTime == 0) {
                printf("%s %d\n", schedProcess[runningIndex].name, schedProcess[runningIndex].pid); 
                waitpid(schedProcess[runningIndex].pid, NULL, 0);
                schedProcess[runningIndex].state = finished;
                finishedNum++;
                runningIndex = -1;
                RRCount = 0;
                for (int i = 0; i < queueLen - 1; i++)
                    readyQueue[i] = readyQueue[i+1];
                queueLen--;

            }
            else if (policy == RR && RRCount == TIMEQUANTUM) {
                /* force to switch process to next one */
                //printf("%d\n", queueLen);
                readyQueue[queueLen] = runningIndex;
                queueLen++;
                for (int i = 0; i < queueLen - 1; i++)
                    readyQueue[i] = readyQueue[i+1];
                queueLen--;
                lowerProcessPriority(schedProcess[runningIndex].pid);
                runningIndex = -1;
                RRCount = 0;
            }
        }
    }
    free(readyQueue);
}
