#include <iostream>
#include <sys/time.h>

//       int gettimeofday(*tv, NULL);
//       struct timeval{
//               time_t      tv_sec;      seconds 
 //              suseconds_t tv_usec;     microseconds 
   //    };
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//       pid_t wait(int *wstatus);

int main(int argc, char** argv){
        pid_t pid;
	pid = fork();
	if (pid == 0){
		execvp(argv[1], argv+1);
		perror("execvp fail");
		return 0;
	}
	timeval t1;
        gettimeofday(&t1, NULL);
	wait(NULL);
	timeval t2;
	gettimeofday(&t2, NULL);
	time_t seconds;
	suseconds_t mikro;
	if (t2.tv_usec - t1.tv_usec < 0){
		seconds = t2.tv_sec - t1.tv_sec - 1;
        	mikro = t2.tv_usec - t1.tv_usec + 1000000;
	}
	else{
		seconds = t2.tv_sec - t1.tv_sec;
		mikro = t2.tv_usec - t1.tv_usec;
	}
	std::cout << seconds << " seconds " << mikro << " microseconds" << std::endl;
        return 0;
}
