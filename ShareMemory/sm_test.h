#ifndef SM_TEST_H
#define SM_TEST_H

#define N 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct {
    pid_t pid;
    char buf[N];
} SHM;

void handler(int signo) {
    printf("get signal\n");
    return;
}


#endif /* end of include guard: SM_TEST_H */

