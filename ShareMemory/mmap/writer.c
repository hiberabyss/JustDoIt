#include <sys/mman.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  

#define BUF_SIZE 100  

int main(int argc, char **argv) {  
    int fd, nread, i;  
    struct stat sb;  
    char *mapped, buf[BUF_SIZE];  

    for (i = 0; i < BUF_SIZE; i++) {  
        buf[i] = '#';  
    }

    if ((fd = open("./data.txt", O_RDWR)) < 0) {  
        perror("open");  
    }

    if ((fstat(fd, &sb)) == -1) {  
        perror("fstat");  
    }  

    if ((mapped = (char *)mmap(NULL, sb.st_size, PROT_READ |   
                    PROT_WRITE, (MAP_SHARED | MAP_PRIVATE), fd, 0)) == (void *)-1) {  
        perror("mmap");  
    }  
  
    close(fd);  
  
    mapped[0] = 'h';  
   
    return 0;  
}
