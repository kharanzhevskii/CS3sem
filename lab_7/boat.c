#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#define N 50
#define M 20
#define K 5
#define NUM_OF_TRIPS 3
// 50 20 5 10
#define NUM_OF_SEMS 6

enum sem{
    tr = 0,
    bt = 1,
    trap_beach = 2,
    trap_boat = 3,
    tickets = 4,
    trip_start = 5,
};

#define SEMOP         semop(id, &semb, 1)

void pass(int id, int pass_number);
void boat(int id);

void semb_init(struct sembuf* semb, unsigned short sem_num, short sem_op, short sem_flg){
    semb->sem_num = sem_num;
    semb->sem_op = sem_op;
    semb->sem_flg = sem_flg;
}

int main(){
    int id = semget(1, NUM_OF_SEMS, IPC_CREAT | 0777);
    if (id < 0) perror("semset died");
    //printf("id = %d\n", id);

    semctl(id, tr, SETVAL, K);
    semctl(id, bt, SETVAL, M);
    semctl(id, trap_beach, SETVAL, 1);
    semctl(id, trap_boat, SETVAL, 1);
    semctl(id, tickets, SETVAL, 0);
    semctl(id, trip_start, SETVAL, 1);


    for(int i = 0; i < N; i++){
        pid_t pid = fork();
        if(pid == 0){
            pass(id, i);
            return 0;
        }
    }
    boat(id);
    semctl(id, 0, IPC_RMID);
    for(int i = 0; i < N; i++) wait(NULL);

    return 0;
}


void boat(int id){
    for(int i = 0; i < NUM_OF_TRIPS; i++){
        struct sembuf semb = {};

        semb_init(&semb, trap_beach, -1, 0);
        if(SEMOP < 0) perror("what???");
        semb_init(&semb, trap_boat, -1, 0);
        if(SEMOP < 0) perror("what???"); // опускаем ladder
        printf("ladder down.\n");

        //sleep(2); //стоим
        semb_init(&semb, bt, -M, 0);
        SEMOP; // ждём, когда все сойдут с корабля
        semb_init(&semb, bt, M, 0);
        SEMOP;


        printf("start selling tickets. rido no%d\n", i);
        semctl(id, tickets, SETVAL, M); // запускаем продажу билетов

        semb_init(&semb, bt, 0, 0);
        if(SEMOP < 0) perror("what???"); // ждём заполнения всех мест на корабле

        semb_init(&semb, tr, -K, 0);
        SEMOP; // ждём когда все уйдут с ladderа
        semb_init(&semb, tr, K, 0);
        SEMOP;

        semb_init(&semb, trap_beach, 1, 0);
        SEMOP;
        semb_init(&semb, trap_boat, 1, 0);
        SEMOP; // поднимаем ladder
        printf("ladder up\n");

        semb_init(&semb, trip_start, -1, 0);
        SEMOP; // отплытие
        printf("go on trip\n");

        sleep(2); //катаемся

        semb_init(&semb, trip_start, 1, 0);
        SEMOP; // прибытие
        printf("return from trip\n");

    }
    printf("done for today\n");
}

void pass(int id, int pass_number){
    for(int i = 0; i < NUM_OF_TRIPS; i++){
        struct sembuf semb = {tickets, -1, 0};
        if (SEMOP == -1) return; // ждём свободные билеты
        printf("passanger no %d bought a ticket\n", pass_number);

        semb_init(&semb, trap_beach, 0, 0);
        SEMOP; // ждём опускания ladderа

        semb_init(&semb, tr, -1, 0);
        SEMOP; // ждём свободные места на ladderе
        printf("passanger no %d got on ladder\n", pass_number);

        semb_init(&semb, trap_boat, 0, 0);
        SEMOP; // убеждаемся, что второй конец ladderа открыт

        semb_init(&semb, bt, -1, 0);
        SEMOP; // заходим на борт
        printf("passanger no %d got on boat\n", pass_number);

        semb_init(&semb, tr, 1, 0);
        SEMOP; // зашли на борт -> на ladderе стало на 1 свободное место больше

        semb_init(&semb, trip_start, 0, 0);
        SEMOP; // ждём отправления и катаемся

        semb_init(&semb, trap_boat, 0, 0);
        SEMOP; // убеждаемся, что можно попасть на ladder

        semb_init(&semb, tr, -1, 0);
        SEMOP; // ждём свободные места на ladderе
        printf("passanger no %d got down from boat\n", pass_number);

        semb_init(&semb, bt, 1, 0);
        SEMOP; // зашли на ladder -> на борту стало на 1 свободное место больше

        semb_init(&semb, trap_beach, 0, 0);
        SEMOP; // убеждаемся, что открыт второй конец ladderа
        printf("passanger no %d got down from ladder\n", pass_number);

        semb_init(&semb, tr, 1, 0);
        SEMOP; // сошли с ladderа
    }
}

