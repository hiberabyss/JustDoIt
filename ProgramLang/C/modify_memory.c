#include <stdio.h>

void hijack() {
    int y = 0;
    printf("&y = %p\n", &y);
    *(&y + 32/sizeof(int)) = 200;
}

int main(int argc, char *argv[]) {
    int x = 100;
    printf("size of int: %d\n", sizeof(int));
    printf("x = %d, &x = %p\n", x, &x);
    hijack();
    printf("x = %d\n", x);
    return 0;
}
