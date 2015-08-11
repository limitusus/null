#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE (4 * 1024 * 1024)

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s pages\n", argv[0]);
        return 1;
    }
    int pages = atoi(argv[1]);
    int pid = getpid();
    void* p = malloc(PAGE_SIZE * pages);
    if (!p) {
        perror("malloc");
        return 1;
    }
    printf("pid: %d\n", pid);
    printf("addr: %p\n", p);
    printf("size: %dkB\n", PAGE_SIZE * pages / 1024);
    printf("sample command:\ngrep -A 1 %lx /proc/%d/smaps\n", (unsigned long)p & 0xFFFFFFFFFFFFFF00, pid);
    sleep(1000);
    return 0;
}
