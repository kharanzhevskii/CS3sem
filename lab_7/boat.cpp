#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <errno.h>

void capitan(){

}

void passanger(){

}

int main(int argc, char** argv){
    int n, m, k, total_rides, check;
    total_rides = 0;
    if (argc < 4) return 0;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    k = atoi(argv[3]);
    int id = semget(IPC_PRIVATE, 3, 0777);
    if (id < 0){
        perror("semget died");
        return 0;
    }
    sembuf sops[3]; // 0 - is boat working or waiting; 1 - seats on boat; 2 - space on ladder; 3 - available tickets
    sops[0].sem_num = 0;
    sops[0].sem_op = 1;
    sops[0].sem_flg = 0;

    sops[1].sem_num = 0;
    sops[1].sem_op = m;
    sops[1].sem_flg = 0;

    sops[2].sem_num = 0;
    sops[2].sem_op = k;
    sops[2].sem_flg = 0;

    check = semop(id, sops, 3);

    pid_t pid = fork();
    if (pid < 0){
        perror("fork died")
        semctl(id, IPC_RMID, 0);
        return 0;
    }

    if (pid == 0){
        capitan();
        return 0;
    }
    for (int i = 0; i < n; i++){
        pid = fork();
        if (pid < 0){
            perror("fork died")
            semctl(id, IPC_RMID, 0);
            return 0;
        }
        if (pid == 0){
            passanger();
            return 0;
        }
    }
    semctl(id, IPC_RMID, 0);
    return 0;
}
