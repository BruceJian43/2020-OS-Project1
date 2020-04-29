#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "schedule.h"
#define POLICYNUM 4

int main()
{
    int  processNum, schedPolicy;
    char schedPolicyType[STRLEN];
    char policyType[POLICYNUM][STRLEN] = {"FIFO", "SJF", "PSJF", "RR"};

    scanf("%s", schedPolicyType);
    scanf("%d", &processNum);
    
    Process *schedProcess = (Process *)malloc(sizeof(Process)*processNum);

    for (int i = 0; i < processNum; i++) {
        scanf("%s %d %d", schedProcess[i].name, &schedProcess[i].readyTime, &schedProcess[i].execTime);
        schedProcess[i].index = i;
        schedProcess[i].state = waiting;
    }
    for (int i = 0; i < POLICYNUM; i++) {
        if (strcmp(schedPolicyType, policyType[i]) == 0) {
            schedPolicy = i;
            break;
        }
    }
    schedulingPreprocess(schedProcess, schedPolicy, processNum);
    
    free(schedProcess);

    return 0;
}
