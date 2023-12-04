#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//       pid_t wait(int *wstatus);

int main(int argc, char** argv){
	pid_t* pid = new pid_t[argc-1];
	int a = 0;
	for (int i = 1; i < argc; i++){
		pid[i-1] = fork();
		if (pid[i-1] == 0){
		       a = atoi(argv[i]);
		       usleep(10000*a);
		       std::cout << a << std::endl;
		       delete[] pid;
		       return 0;
		}
	}
	for (int i = 1; i < argc; i++){
		wait(NULL);
	}
	delete[] pid;
	return 0;
}	
