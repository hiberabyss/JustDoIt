#include <sys/ptrace.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SHOW(call) ({ int _ret = (int)(call); printf("%s -> %d\n", #call, _ret); if (_ret < 0) { perror(NULL); }})

char changeme[] = "This is  a test";

int main (void)
{
    pid_t pid = fork();
    int ret;
    int i;
    union {
        char cdata[8];
        int64_t data;
    } u = { "Hijacked" };

    switch (pid) {
        case 0: /* child */
            sleep(2);
            printf("Children Message: %s\n", changeme);
            exit(0);

        case -1:
            perror("fork");
            exit(1);
            break;

        default: /* parent */
            SHOW(ptrace(PTRACE_ATTACH, pid, 0, 0));
            SHOW(ptrace(PTRACE_POKEDATA, pid, changeme, u.data));
            SHOW(ptrace(PTRACE_CONT, pid, 0, 0));
            printf("Parent Message: %s\n", changeme);
            wait(NULL);
            break;
    }

    return 0;
}
