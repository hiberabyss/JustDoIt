#include "sm_test.h"

int main()
{
    key_t key;
    int shmid;
    SHM *p;
    pid_t other_pid;

    if ((key = ftok(".", 'm')) < 0) {
        perror("fail to ftok");
        exit(-1);
    }

    signal(SIGUSR1, handler);
    if ((shmid = shmget(key, sizeof(SHM), 0666 | IPC_CREAT | IPC_EXCL)) < 0
            && errno != EEXIST) {
        perror("fail to shmget");
        exit(-1);
    }

    // reader has already started
    if (shmid < 0) {
        shmid = shmget(key, sizeof(SHM), 0666);
        p = (SHM *)shmat(shmid, NULL, 0);
        other_pid = p->pid;
        p->pid = getpid();
        kill(other_pid, SIGUSR1);
    } else {
        p = (SHM *)shmat(shmid, NULL, 0);
        p->pid = getpid();
        pause();
        other_pid = p->pid;
    }

    while ( 1 ) {
        printf("write to shm : ");
        fgets(p->buf, N, stdin);
        kill(other_pid, SIGUSR1);
        if (strcmp(p->buf, "quit\n") == 0) break;
        pause();
    }

    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);          //  删除共享内存

    return 0;
}
