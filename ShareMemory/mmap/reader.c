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
  
    /* 获取文件的属性 */  
    if ((fstat(fd, &sb)) == -1) {  
        perror("fstat");  
    }  
  
    /* 将文件映射至进程的地址空间 */  
    if ((mapped = (char *)mmap(NULL, sb.st_size, PROT_READ |   
                    PROT_WRITE, (MAP_PRIVATE|MAP_SHARED), fd, 0)) == (void *)-1) {  
        perror("mmap");  
    }  
  
    /* 文件已在内存, 关闭文件也可以操纵内存 */  
    close(fd);  
      
    while (1) {  
        printf("%s\n", mapped);  
        sleep(2);  
    }  
  
    return 0;  
}  
