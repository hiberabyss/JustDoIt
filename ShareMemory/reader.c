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
    if ((shmid = shmget(key, sizeof(SHM), 0666|IPC_CREAT|IPC_EXCL)) < 0)
    {
        if (errno != EEXIST) {
            perror("fail to shmget");
            exit(-1);
        }

        // writer has already started
        shmid = shmget(key, sizeof(SHM), 0666);
        p = (SHM *)shmat(shmid, NULL, 0);
        other_pid = p->pid;
        p->pid = getpid();//把自己的pid写到共享内存
        kill(other_pid, SIGUSR1);
    } else {
        p = (SHM *)shmat(shmid, NULL, 0);
        p->pid = getpid();
        // wait writer start to get its pid
        pause();
        other_pid = p->pid;
    }

    while ( 1 ) {
        pause();
        if (strcmp(p->buf, "quit\n") == 0) exit(0);//输入"quit结束"
        printf("read from shm : %s", p->buf);
        kill(other_pid, SIGUSR1);//向写进程发SIGUSR1信号
    }

    return 0;
}
