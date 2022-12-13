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

#define COUNT 5
static int semId;

void killProc() {
    semctl(semId, IPC_RMID, 0);
    kill(0, SIGKILL);
    exit(0);
}

int main(int argc, char **argv) {
    int cntUnit = 1;
    char idToSend[30], countToSend[10];
    struct sembuf sops[2] = {
            {2, -1, 0},
            {3, -1, 0}
    };
    key_t IPCkey;
    if ((IPCkey = ftok("/mnt/d/prak_5", 's')) == -1) {
        printf("Can't get key");
        exit(1);
    }
    if ((semId = semget(IPCkey, 4, IPC_CREAT | 0666)) == -1) {
        printf("Can't get semaphore array\n");
        exit(1);
    }
    sprintf(idToSend, "%d", semId);
    sprintf(countToSend, "%d", COUNT);
    signal(SIGINT, killProc);
    if (!fork()) {
        execl("unit", "a", idToSend, countToSend, NULL);
        perror("a");
        exit(1);
    }
    if (!fork()) {
        execl("unit", "b", idToSend, countToSend, NULL);
        perror("b");
        exit(1);
    }
    if (!fork()) {
        execl("unit", "c", idToSend, countToSend, NULL);
        perror("c");
        exit(1);
    }
    if (!fork()) {
        execl("module", "./module", idToSend, countToSend, NULL);
        perror("module");
        exit(1);
    }
    for (; cntUnit <= COUNT; cntUnit++) {
        if (semop(semId, sops, 2) == -1) {
            perror(argv[0]);
            exit(1);
        }
        printf("%s: is processing #%d detail\n", argv[0], cntUnit);
    }
    killProc();
    return 0;
}


