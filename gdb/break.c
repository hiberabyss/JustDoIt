#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/user.h>

const int long_size = sizeof(long);

void getdata(pid_t child, long addr, 
             char *str, int len)
{   
    char *laddr;
    int i, j;
    union u {
            long val;
            char chars[long_size];
    }data;

    i = 0;
    j = len / long_size;
    laddr = str;

    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child, 
                          addr + i * 4, NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, child, 
                          addr + i * 4, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '';
}

void putdata(pid_t child, long addr, 
             char *str, int len)
{   
    char *laddr;
    int i, j;
    union u {
            long val;
            char chars[long_size];
    }data;

    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, child, 
               addr + i * 4, data.val);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, child, 
               addr + i * 4, data.val);
    }
}

int main(int argc, char *argv[])
{   
    pid_t traced_process;
    struct user_regs_struct regs, newregs;
    long ins;
    /* int 0x80, int3 */
    char code[] = {0xcd,0x80,0xcc,0};
    char backup[4];
    if(argc != 2) {
        printf("Usage: %s <pid to be traced> ", 
               argv[0], argv[1]);
        exit(1);
    }
    traced_process = atoi(argv[1]);
    ptrace(PTRACE_ATTACH, traced_process, 
           NULL, NULL);
    wait(NULL);
    ptrace(PTRACE_GETREGS, traced_process, 
           NULL, ins);
    /* Copy instructions into a backup variable */
    getdata(traced_process, regs.eip, backup, 3);
    /* Put the breakpoint */
    putdata(traced_process, regs.eip, code, 3);
    /* Let the process continue and execute 
       the int 3 instruction */
    ptrace(PTRACE_CONT, traced_process, NULL, NULL);
    wait(NULL);
    printf("The process stopped, putting back "
           "the original instructions ");
    printf("Press <enter> to continue ");
    getchar();
    putdata(traced_process, regs.eip, backup, 3);
    /* Setting the eip back to the original 
       instruction to let the process continue */
    ptrace(PTRACE_SETREGS, traced_process, 
           NULL, ins);
    ptrace(PTRACE_DETACH, traced_process, 
           NULL, NULL);
    return 0;

}
