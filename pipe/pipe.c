#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define PAGESIZE page_size
#define PAGES 32

int main(void) {
    int pid;
    int pipes[2];
    int ret;
    long page_size = sysconf(_SC_PAGESIZE);
    ret = pipe(pipes);
    if (ret < 0) {
        perror("pipe");
        exit(1);
    }
    int j;
    for (j = 0; j < 2; j++) {
        printf("pipes[%d]: %d\n", j, pipes[j]);
    }
    char* buf;
    buf = malloc(PAGES * PAGESIZE);
    if (!buf) {
        perror("malloc");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) { // child
        close(pipes[1]);
#if DO_SLEEP
        printf("sleep start\n");
        sleep(100);
#endif
        ret = -1;
        while (ret != 0) {
            ret = read(pipes[0], buf, PAGES * PAGESIZE);
            if (ret < 0) {
                perror("read");
                exit(2);
            }
        }
        close(pipes[0]);
#if DO_SLEEP
        printf("sleep end\n");
#endif
        exit(0);
    } else { // parent
        close(pipes[0]);
        printf("child: %d\n", pid);
        int i;
        for (i = 0; i < PAGES; i++) {
            printf("writing %d: %d\n", i, pipes[1]);
            ret = write(pipes[1], buf + PAGESIZE * i, PAGESIZE);
            if (ret < 0) {
                perror("write");
                kill(pid, 9);
                exit(2);
            }
        }
        close(pipes[1]);
    }
    int status;
    ret = wait(&status);
    if (ret < 0) {
        perror("wait");
        kill(pid, 9);
        exit(1);
    }
    exit(0);
}
