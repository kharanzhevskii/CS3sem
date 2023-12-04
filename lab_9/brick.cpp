// #include <sys/types.h>
// #include <iostream>
// #include <sys/ipc.h>
// #include <sys/sem.h>
#include <stdlib.h>
// #include <sys/shm.h>
// #include <unistd.h>
#include <errno.h>
#include <pthread.h>

struct Wall{
    pthread_mutex_t m;
    pthread_cond_t cf, cs;
    int f, s, fw, sw, n, M;
    void init(Wall &wall, int n, int M);
    void finit(Wall &wall);
};

void f_put_brick(int M);
void s_put_brick(int M);

void Wall::init(Wall &wall, int n, int M){
    wall.m = PTHREAD_MUTEX_INITIALIZER;
    wall.n = n;
    wall.M = M;
}

void Wall::finit(Wall &wall){
    pthread_mutex_destroy(&wall.m);
}

int main(int argc, char** argv){
    Wall wall;
    int n = atoi(argv[1]);
    int M = atoi(argv[2]);
    int check;
    pthread_t f_tid, s_tid;
    wall.init(&wall, n, M);
    check = pthread_create(&f_tid, NULL, f_put_brick(M), (void*)M);
    if (check != 0){
        perror("father didn't come");
        return 1;
    }
    check = pthread_create(&s_tid, NULL, s_put_brick(M), (void*)M);
    if (check != 0){
        perror("son didn't come");
        return 1;
    }
    wall.finit(&wall);
    return 0;
}