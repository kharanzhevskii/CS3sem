#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <vector>


#define PIPE_BUF 4096

int count(int argc, char** argv){
    int counter = 0;
    for (int i = 1; i < argc; i++){
            if (argv[i] == "|") counter++;
    }
    return counter;
}

std::vector<std::vector<std::string>> parser(int argc, char** argv){
    std::vector<std::vector<std::string>> mylist;
    mylist.resize(count(argc, argv));
    int k = 0;
    for (int i = 1; i < argc; i++){
        if (argv[i] != "|"){
            mylist[k].push_back(argv[i]);
        }
        else{
            k++;
        }        
    }
    return mylist;
}

int main(int argc, char** argv){
    auto mylist = parser(argc, argv);
    pid_t pid;
    int check0, check1;
    int* fds0 = new int[2];
    int* fds1 = new int[2];
    for (int i = 0; i < count(argc, argv)+1; i++){
        check0 = pipe(fds0);
        check1 = pipe(fds1);
	    if (check0 < 0 or check1 < 0){
		    perror("pipe died");
            delete[] fds0;
            delete[] fds1;
		    return 1;
	    }
        pid = fork();
        if (pid == 0){
            check0 = dup2(fds0[0], 0);
            check1 = dup2(fds1[1], 1);
            close(fds0[0]);
	        close(fds1[1]);
	        if (check0 < 0 or check1 < 0) {
		        perror("dup2 died");
                delete[] fds0;
                delete[] fds1;
		        return 1;
            }
	        delete[] fds0;
            delete[] fds1;
	        execvp(mylist[i][0].c_str(), argv);
	        perror("execvp died");
            return 1;
        }
	    else{
		    for (int j = 0; j < i+1; j++){ wait(NULL); }
	    	//close(fds1[1]);
        }
		delete[] fds0;
        delete[] fds1;
        }
        return 0;
}
