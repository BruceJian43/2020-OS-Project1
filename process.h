#ifndef PROCESS_H
#define PROCESS_H

#define STRLEN 32  
enum STATE {
    waiting = 0,
    ready,
    finished
};
typedef struct {
    char name[STRLEN];
    int index;
    int readyTime;
    int execTime;
    int state; 
    pid_t pid;
}Process;

#endif
