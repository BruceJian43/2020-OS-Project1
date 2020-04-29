#ifndef CPUSTATE_H
#define CPUSTATE_H

#define scheduleCPU 0
#define processCPU 1
#define SYSCALL_GETTIME 333
#define SYSCALL_PRINTK	334

void assignProcessToOneCpu(pid_t pid, int cpuNum);
void raiseProcessPriority(pid_t pid);
void lowerProcessPriority(pid_t pid);
pid_t execProcess(Process currentProcess);


#endif
