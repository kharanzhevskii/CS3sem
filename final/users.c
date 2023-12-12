#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PIPE_BUF 4096
#define TOTAL 5

int main(int argc, char** argv) {
    int fds[2] = {};
	if (pipe(fds) < 0){
		perror("pipe died");
		exit(1);
    }
    
    pid_t pid = fork();
	if (pid == 0){
        int check = dup2(fds[1], 1);
		close(fds[1]);
		if (check < 0) {
			perror("dup2 died");
			exit(1);
	    }
		execvp(argv[1], argv+1);
		perror("execvp fail");
		exit(1);
	}
    else{
        wait(NULL);
        //fflush();
        int check = dup2(fds[0], 0);
		close(fds[0]);
		if (check < 0) {
			perror("dup2 died");
			exit(1);
	    }
        char buf[PIPE_BUF] = {};
        char users[50][100] = {};
        bool reading = 0;
        int name_number = 0;
        int letter_number = 0;

        ssize_t red = read(0, buf, PIPE_BUF);
        
        for (int i = 0; i < red; i++){
            if (buf[i] == '\n' && i != red-1) reading = 1;
            if (reading && buf[i] == ' '){
                reading = 0;
                name_number++;
                letter_number = 0;
            }
            if (reading && buf[i] != '\n'){
                users[name_number][letter_number] = buf[i];
                letter_number++;
            }
        }

        struct answer{
            char username[100];
            int repeats;
        };
        struct answer ans[TOTAL] = {};
        bool found = 0;
        int unique_names = 0;
        for (int i = 0; i < name_number-1; i++){
            for (int j = 0; j < TOTAL; j++){
                if (strcmp(users[i], ans[j].username) == 0){
                    ans[j].repeats++;
                    found = 1;
                }
            }
            if (!found){
                //ans[unique_names].username = users[i];
                strcpy(ans[unique_names].username , users[i]);
                ans[unique_names].repeats = 1;
                unique_names++;
            }
            found = 0;
        }
        
        for (int i = 0; i < unique_names; i++){
            printf("%s\t%d\n", ans[i].username, ans[i].repeats);
        }
        exit(0);
    }
}