#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

int reader() {

}

int writer() {

}

int main() {
	int id0 = shmget(IPC_PRIVATE, 4096, 0700);
	if (id0 == -1) {
		perror("first shmget died");
		return 1;
	}
	void* ptr0 = shmat(id, NULL, 0);
	if (*ptr0 == -1) {
                perror("first shmat died");
                return 1;
	}
	//int id1 = shmget(IPC_PRIVATE, 4096, 0700);
	//if (id1 == -1) {
        //        perror("second shmget died");
        //        return 1;
	//}
        //void* ptr1 = shmat(id, NULL, 0);
	//if (*ptr1 == -1) {
        //        perror("second shmat died");
        //        return 1;
        //}
	//char* p0 = ptr0;
	//char* p1 = ptr1;
	char** buf = ptr0;

	int semid = semget(IPC_PRIVATE, 2, 0777);
    	if (semid == -1){
        	perror("semget died");
        	return 1;
    	}
    	sembuf sops[5]; // 0 - reader-1; 1 - writer+1; 2 - writer-1; 3 - reader+1; 
	sops[0] = {0, -1, 0};
	sops[3] = {0, 1, 0};
	sops[1] = {1, 1, 0};
	sops[2] = {1, -1, 0};
	sops[4] = {1, 0, 0};
    	
	int check = semop(semid, sops[1], 1);
	int check = semop(semid, sops[4], 1);
	if (check == -1){
		perror("semop died");
		semctl(semid, IPC_RMID, NULL);
		return 1;
	}

	
	int d0 = shmdt(ptr0);
	if (d0 == -1) {
                perror("first shmdt died, memory lost");
                return 1;
        }
	int d1 = shmdt(ptr1);
	if (d1 == -1) {
                perror("second shmdt died, memory lost");
                return 1;
        }
	d0 = shmctl(id0, IPC_RMID, NULL);
	if (d0 == -1) {
                perror("first shmctl died, panic");
                return 1;
        }
	d1 = shmctl(id1, IPC_RMID, NULL);
	if (d1 == -1) {
                perror("second shmdt died, panic");
                return 1;
        }
	return 0;
}
