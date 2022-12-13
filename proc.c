#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char **argv) {
    int cntUnit = 1;
    int index = argv[0][0] - 'a';
    int sleepTime[] = {2, 3, 4};
    int semID = atoi(argv[1]);
    struct sembuf sops = {index, 1, 0};
    setbuf(stdout, NULL);
    for (;cntUnit <= atoi(argv[2]); cntUnit++) {
        sleep(sleepTime[index]);
        printf("%s: is processing #%d unit\n", argv[0], cntUnit);
        if (semop(semID, &sops, 1) == -1) {
            perror(argv[0]);
            exit(1);
        }
    }
}