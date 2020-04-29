#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <time.h>
#include "process.h"
#include "schedule.h"
#include "cpustate.h"


void assignProcessToOneCpu(pid_t pid, int cpuNum) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpuNum, &set);
    int ret = sched_setaffinity(pid, sizeof(set), &set);
    if (ret < 0)
        printf("assign error\n");
}
void raiseProcessPriority(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 99;
    int ret = sched_setscheduler(pid, SCHED_FIFO, &param);
    if (ret < 0)
        printf("raise error\n");
}
void lowerProcessPriority(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 0;
    int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
    if (ret < 0)
        printf("lower error\n");
}
pid_t execProcess(Process currentProcess){
    struct timespec start, end;
    syscall(SYSCALL_GETTIME, &start);
    int pid = fork();
    if (pid == 0) {
        assignProcessToOneCpu(getpid(), processCPU);
        for (int i = 0; i < currentProcess.execTime; i++)
            UNIT_TIME();
       	syscall(SYSCALL_GETTIME, &end);
        syscall(SYSCALL_PRINTK, getpid(), &start, &end); 
        _exit(0);
    }
    lowerProcessPriority(pid);
    return pid;
}
