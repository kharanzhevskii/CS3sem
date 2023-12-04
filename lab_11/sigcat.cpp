#include <termios.h>
#include <signal.h>
// #include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
// #include <errno.h>
#define SIZE 4096

char wbuf;
int inc = 0;
bool bit;
pid_t childpid;
pid_t parentpid;
char buf[SIZE];
int i = 0;
int j = 0;

void handle_write(int sig);
void handle_read(int sig);


int main() {
    pid_t checkpid = fork();
    if (checkpid == 0){
        pid_t ppid = getppid();
        parentpid = ppid;
        ssize_t red = read(0, buf, SIZE);
        signal(SIGUSR2, handle_read);
        for (; i < red; i++){
            for (; j < 8; j++){
                pause();
            }
        }
    }    
    else{
        printf("what???\n");
        childpid = checkpid;
        signal(SIGUSR1, handle_write);
        signal(SIGUSR2, handle_write);
        kill(checkpid, SIGUSR2);
        while (1){
            pause();
        }
    }
    // free(buf);
    return 0;
}

void handle_write(int sig){
    inc ++;
    if (sig == SIGUSR1) bit = 0;
    else bit = 1;
    printf("%d\n", bit);
    wbuf = (wbuf << 1) || bit;
    if (inc == 8) {
        write(1, &wbuf, 1);
        inc = 0;
    }
    kill(childpid, SIGUSR2);
    printf("hi to son\n");
}

void handle_read(int sig){
    bool symbl = (buf[i] >> j) && 1;
    if (symbl == 0) kill(parentpid, SIGUSR1);
    else kill(parentpid, SIGUSR2);
}