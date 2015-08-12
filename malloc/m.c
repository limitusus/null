#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define STEP 1

int main(int argc, char** argv) {
    long page_size = sysconf(_SC_PAGESIZE);
    if (argc < 3) {
        fprintf(stderr, "usage: %s pages [step]\n", argv[0]);
        return 1;
    }
    int pages = atoi(argv[1]);
    int step = STEP;
    if (argc >= 2) {
        step = atoi(argv[2]);
    }
    int pid = getpid();
    char* p = malloc(page_size * pages);
    if (!p) {
        perror("malloc");
        return 1;
    }
    void* p_align = (unsigned long)p & (unsigned long)(0xFFFFFFFFFFFFF000);
    int r = madvise(p_align, page_size * (pages + 1), MADV_RANDOM);
    if (r < 0) {
        perror("madvise");
        return 1;
    }
    long c;
    for (c = 0; c < pages; c += step) {
        int idx = c * page_size;
        printf("%ld %p\n", c, &p[idx]);
        p[idx] = 1;
    }
    printf("page_size: %ld\n", page_size);
    printf("step_bytes: %ld\n", page_size * step);
    printf("pid: %d\n", pid);
    printf("addr: %p\n", p);
    printf("size: %ldkB\n", page_size * pages / 1024);
    printf("sample command:\ngrep -A 20 %lx /proc/%d/smaps\n", (unsigned long)p & 0xFFFFFFFFFFFFFF00, pid);
    sleep(1000);
    return 0;
}
