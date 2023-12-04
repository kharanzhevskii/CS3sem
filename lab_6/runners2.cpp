//#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
//#include <sys/time.h>
#include <sys/wait.h>

int N, id; //number of runners	FIFO id
// struct msgbuf{
//     long mtype;       /* message type, must be > 0 */
//     //char mtext[1];    /* message data - not needed*/
// };

void judge() {
	msgbuf buf = {};
    for (int i = 1; i < N+1; i++){
        if (msgrcv(id, (void*)(&buf), 0, i, 0) < 0) perror("judge 1st rcv failed");
        printf("%d guy is here, judge confirms\n", i);
    }
	buf = {};
    buf.mtype = N+1;
    if (msgsnd(id, (const void*)(&buf), 0, 0) < 0) perror("judge 1st snd failed");
    printf("judge gave the stick to 1\n");
    if (msgrcv(id, (void *)(&buf), 0, N+N+1, 0) < 0) perror("judge n st rcv failed");
    printf("judge got the stick from %d\n", N);
}

void runner(int i) {
		msgbuf buf = {};
		buf.mtype = i;
        if (msgsnd(id, (const void*)(&buf), 0, 0) < 0) perror("runner snd for check failed");
        if (msgrcv(id, (void*)(&buf), 0, N+i, 0) < 0) perror("runner rcv failed");
        printf("%d got the stick from %d\n", i, i-1);
		buf = {};
        buf.mtype = N+i+1;
        if (msgsnd(id, (const void*)(&buf), 0, 0) < 0) perror("i runner snd for next failed");
}

int main(int argc, const char** argv) {
	key_t key = ftok(argv[0], 'a');
	id = msgget(key, IPC_CREAT | 0777);
	if (argc > 1) N = atoi(argv[1]);
	else N = 5;
	// int n, id, check;
	pid_t pid;
	// //long long* buf = new long long[8];
	// msgbuf buf = {};

	// //long long** list_buf = new long long[n];
	// id = msgget(IPC_PRIVATE, 0777);
	pid = fork();
	if (pid == 0){
		judge();
		return 0;
	}
	for (int i = 1; i < N+1; i++){
		pid = fork();
		if (pid == 0){
            runner(i);
			return 0;
		}
	}
	for (int j = 0; j < N; j++) wait(NULL);

    msgctl(id, IPC_RMID, 0);
	return 0;
}
