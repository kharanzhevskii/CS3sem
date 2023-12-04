#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

void resize(int q){
    winsize W;
    printf("\e[H\e[J\n");
    int err = ioctl(0, TIOCGWINSZ, &W);
    printf("\e[%d;%dHrows:%d\tcols:%d\n", W.ws_row/2, W.ws_col/2, W.ws_row, W.ws_col);
    //printf("\e[Hcols:%d\trows:%d\n", W.ws_row, W.ws_col);
}

void die(int q){
    winsize W;
    printf("\e[H\e[J\n");
    int err = ioctl(0, TIOCGWINSZ, &W);
    printf("\e[%d;%dHBYE\n", W.ws_row/2, W.ws_col);
    exit(0);
}

int main() {
    signal(SIGWINCH, resize);
    signal(SIGINT, die);
    getchar();
    return 0;
}