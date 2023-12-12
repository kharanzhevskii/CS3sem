#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

//F_GETPIPE_SZ
int main() {
    ssize_t w;
    long inc = 0;
    char buf[1];
    buf[0] = 'q';
    int fds[2] = {};
    if (pipe2(fds, O_NONBLOCK) < 0){
        perror("pipe2 dead");
        return 0;
    }
    pid_t pid = fork();
    if (pid == 0){
        // do{
        // w = write(fds[1], &buf, 1);
        // inc++;} 
        while (write(fds[1], &buf, 1) > 0) {
            inc++;
        }
        printf("%ld\n", inc);
        return 0;
    }
    return 0;
}

