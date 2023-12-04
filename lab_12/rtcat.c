#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4096

pid_t childpid;
pid_t parentpid;
char buf[SIZE];
union sigval data;
siginfo_t info;
struct timespec timeout = {0, 1000};
int check;
union sigval waste[1];

void handle(int sig, siginfo_t* info, void* s){
    waste[0] = info->si_value;
    write(1, waste, 1);
}

void die(int sig){
    exit(0);
}

int main() {
    pid_t checkpid = fork();
    if (checkpid == 0){
        parentpid = getppid();
        while (1) {
            ssize_t red = read(0, buf, SIZE);
            if (red == 0){
                sigqueue(parentpid, SIGRTMIN+1, data);
                exit(0);
            }

            for (int i = 0; i < red; i++){
                data.sival_int = buf[i];
                do{
                check = sigqueue(parentpid, SIGRTMIN, data);
                } while (check < 0);    
            }
        }  
    }
    else{
        sigset_t m;
        sigfillset(&m);
        sigdelset(&m, SIGRTMIN);
        sigdelset(&m, SIGINT);
        sigdelset(&m, SIGTERM);
        sigdelset(&m, SIGKILL);
        sigdelset(&m, SIGSTOP);

        sigset_t mend;
        sigfillset(&mend);
        sigdelset(&mend, SIGRTMIN+1);
        
        struct sigaction act = {};
        act.sa_sigaction = handle;
        act.sa_mask = m;
        act.sa_flags = SA_SIGINFO; //|| SA_RESTART;

        struct sigaction actend = {};
        actend.sa_handler = die;
        actend.sa_mask = mend;
        if (sigaction(SIGRTMIN, &act, NULL) < 0){
            perror("SIGRTMIN");
        };

        if (sigaction(SIGRTMIN+1, &actend, NULL) < 0){
            perror("SIGRTMIN+1");
        };

        while (1){
            sigtimedwait(&m, &info, &timeout);           
        }
    }
    return 0;
}
