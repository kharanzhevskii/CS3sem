void judge(int id, int n, msgbuf* buf) {
        for (int i = 0; i < n; i++){
                if (msgrcv(id, (void* buf), 0, 0, 0) < 0) perror("judge 1st rcv failed");
                printf("%d is here, judge confirms", i);
        }
        buf->mtype = -1;
        if (msgsnd(id, (void* buf), 0, 0) < 0) perror("judge 1st snd failed");
        printf("judge gave the stick to 1");
        if (msgrcv(id, (void* buf), 0, n, 0) < 0) perror("judge n st rcv failed");
        printf("judge got the stick from n");
}

void runner(int id, int i, msgbuf* buf) {
        if (msgsnd(id, (void* buf), 0, 0) < 0) perror(" runner snd for check failed");
        if (msgrcv(id, (void* buf), 0, i-1, 0) < 0) perror(" runner rcv failed");
        printf("%d got the stick from %d", i, i-1);
        buf->mtype = i;
        if (msgsnd(id, (void* buf), 0, 0) < 0) perror("i runner snd for next failed");
}