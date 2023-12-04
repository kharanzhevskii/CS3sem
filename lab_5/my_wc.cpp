#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


#define PIPE_BUF 4096

int main(int argc, char** argv){
        pid_t pid;
	int* fds = new int[2];
        int check = pipe(fds);
	if (check < 0) {
		perror("pipe died");
		return 1;
	}
        pid = fork();
        if (pid == 0){
                   int check = dup2(fds[1], 1);
		   close(fds[1]);
		   if (check < 0) {
			   perror("dup2 died");
			   return 1;
	           }
		   delete[] fds;
		   execvp(argv[1], argv+1);
		   perror("execvp died");
                   return 1;
        }
	else{
		char* buf = new char[PIPE_BUF];
		
		close(fds[1]);
		ssize_t red, counter;
		int lines = 0;
                int words = 0;
                int symbls = 0;
		bool overload = 0;
		NOMEM:
	        red = read(fds[0], buf, PIPE_BUF);
		if (red < 0){
			delete[] buf;
        	        perror("read died");
                	return 1;
        	}
		counter = red;
		if (counter == PIPE_BUF) {
			overload = 1;
		}
		while (red > 0 && counter < PIPE_BUF && overload == 0){
			red = read(fds[0], buf, PIPE_BUF);
			if (red < 0){
				delete[] buf;
                        	perror("read died");
                        	return 1;
			} //EOF is when red == 0, so it just skips 'else' part
			else{
				counter += red;
				if (counter == PIPE_BUF) {
					overload = 1;
		     		}
			}
                }
                for (int i = 0; buf[i] != NULL; i++){
			symbls++;
			if (buf[i] == '\n') {
				lines++;
				words++;
			}
			if (buf[i] == ' ' && (buf[i+1] != ' ' or buf[i+1] != '\n')) words++;
			if (buf[i+1] == NULL && buf[i] != '\n'){
				lines++;
                                words++;
		      	}	      
		}
		if (overload){
			delete[] buf;
                        char* buf = new char[PIPE_BUF];
			overload = 0;
			counter = 0;
                        goto NOMEM;
		}
		std::cout << lines << " " << words << " " << symbls << std::endl;
		delete[] buf;
        	delete[] fds;
        }
        return 0;
}
