#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4096

pid_t childpid;
pid_t parentpid;

union sigval data;
siginfo_t info;
struct timespec timeout = {1, 0};
int check;
union sigval waste[1];

void handle(int sig){
    //printf("%d\n", info->si_value);
}

void die(int sig){
    exit(0);
}

int main() {
    char buf[1];
    buf[0] = 'x';
    int fds[2] = {};
    if (pipe2(fds, 0) < 0){
       perror("pipe2 dead");
       return 0;
    }
    pid_t checkpid = fork();
    if (checkpid == 0){
        //int inc = 0;
        parentpid = getppid();
        // sigset_t m;
        // sigempset(&m);
        // sigdelset(&m, SIGINT);
        // struct sigaction act = {};
        // act.sa_handler = die;
        // act.sa_mask = m;
        // data.sival_int = 0;
        // if (sigaction(SIGINT, &act, NULL) < 0){
        //     perror("SIGRTMIN");
        // };
        data.sival_int = 0;
        while (write(fds[1], buf, 1) > 0) {
            if (sigqueue(parentpid, SIGRTMIN, data) < 0) perror("what");
        }
        // data.sival_int = inc;
        // do{
        // check = sigqueue(parentpid, SIGRTMIN, data);
        // } while (check < 0);     
    }
    else{
        int inc = 0, check;
        sigset_t m;
        sigfillset(&m);
        sigdelset(&m, SIGRTMIN);
        sigdelset(&m, SIGINT);
        sigdelset(&m, SIGTERM);
        sigdelset(&m, SIGKILL);
        sigdelset(&m, SIGSTOP);

        sigset_t ma;
        sigemptyset(&ma);
        sigaddset(&ma, SIGRTMIN);
        
        
        struct sigaction act = {};
        act.sa_handler = handle;
        act.sa_mask = m;
        act.sa_flags = SA_SIGINFO; //|| SA_RESTART;

        if (sigaction(SIGRTMIN, &act, NULL) < 0){
            perror("SIGRTMIN");
        };

        while (1) {
            if (sigtimedwait(&ma, &info, &timeout) > 0) inc++;
            else{
                kill(checkpid, SIGTERM);
                printf("%d\n", inc);
                return 0; 
            }
            
        }        
    }
    return 0;
}