#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define STEP 1
#define DO_MADVISE 1
#define GREP_LINES 13

int main(int argc, char** argv) {
    long page_size = sysconf(_SC_PAGESIZE);
    int touched_pages = 0;
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
    void* p_align = (void*)((unsigned long)p & (unsigned long)(0xFFFFFFFFFFFFF000));
#if DO_MADVISE
    int r = madvise(p_align, page_size * (pages + 1), MADV_RANDOM);
    if (r < 0) {
        perror("madvise");
        return 1;
    }
#endif
    long c;
    for (c = 0; c < pages; c += step) {
        int idx = c * page_size;
        // printf("%ld %p\n", c, &p[idx]);
        p[idx] = 1;
        touched_pages++;
    }
    printf("page_size: %ld\n", page_size);
    printf("step_bytes: %ld\n", page_size * step);
    printf("pid: %d\n", pid);
    printf("addr: %p\n", p);
    printf("size: %ldkB\n", page_size * pages / 1024);
    printf("touched pages: %d (%ld kbytes)\n", touched_pages, touched_pages * page_size / 1024);
    printf("sample command:\ngrep -A %d %lx /proc/%d/smaps\n", GREP_LINES, (unsigned long)p & 0xFFFFFFFFFFFFFF00, pid);
    sleep(1000);
    return 0;
}
