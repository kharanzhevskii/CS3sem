#include <signal.h>
#include <errno.h>
#define SIZE 4096

pid_t childpid;
pid_t parentpid;
char buf[SIZE];
union sigval data;
siginfo_t info;

char waste[1];

void handle(int sig, siginfo_t* info, void* s){
    waste = info->si_code;
    write(1, waste, 1);
}

int main() {
    pid_t checkpid = fork();
    if (checkpid == 0){
        pid_t ppid = getppid();
        parentpid = ppid;
        ssize_t red = read(0, buf, SIZE);
        for (int i = 0; i < red; i++){
            data.sival_int = buf[i];
            while (sigqueue(parentpid, SIGUSR1, data) < 0){
                perror("idk");
            }
        }
    }
    else{
        sigset_t m;
        sigfillset(&m);
        sigdelset(&m, SIGUSR1);
        sigaction act;
        act.sa_sigaction = handle;
        sigaction(SIGUSR1, &act, NULL);
        while (1){
            sigwaitinfo(&m, &info);           
        }
    }
    return 0;
}

