#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>

//struct msgbuf {
//	long mtype;        message type, must be > 0 
//};

void judge(int id, int n, void* buf) {
	for (int i = 0; i < n; i++){
		if (msgrcv(id, buf, 0, 0, 0) < 0) perror("judge 1st rcv failed");
		printf("%d is here, judge confirms", i);
	}
	//static_cast<msgbuf>(*buf);
	*(msgbuf*)buf.mtype = 0;
	if (msgsnd(id, buf, 0, 0) < 0) perror("judge 1st snd failed");
	printf("judge gave the stick to 1");
	if (msgrcv(id, buf, 0, n, 0) < 0) perror("judge n st rcv failed");
	printf("judge got the stick from n");
}

void runner(int id, int i, void* buf) {
	if (msgsnd(id, buf, 0, 0) < 0) perror(" runner snd for check failed");
	if (msgrcv(id, buf, 0, i-1, 0) < 0) perror(" runner rcv failed");
	printf("%d got the stick from %d", i, i-1);
	buf->mtype = i;
	if (msgsnd(id, buf, 0, 0) < 0) perror("i runner snd for next failed");
}

int main() {
	int n, id, check;
	pid_t pid;
	std::cin >> n;
	//long long* buf = new long long[8];
	msgbuf buf {};

	//long long** list_buf = new long long[n];
	id = msgget(IPC_PRIVATE, 0777);
	pid = fork();
	if (pid == 0){
		judge(id, n, &buf);
		return 0;
	}
	for (int i = 1; i < n+1; i++){
		pid = fork();
		if (pid == 0){
                        runner(id, i, &buf);
			return 0;
		}
	}
	for (int j = 0; j < n+1; j++) wait(NULL);
       	msgctl(id, IPC_RMID, 0);
	//delete[] buf;
	return 0;
}
