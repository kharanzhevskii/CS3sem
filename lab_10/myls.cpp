#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <string.h>

void ls(char* name){
    DIR* dp = opendir(name);
    if (dp == NULL) std::cout << name;
    dirent* ep;
    while ((ep = readdir(dp)) != NULL){
        if (ep->d_type == DT_DIR && ep->d_name != "." && ep->d_name != ".."){
            std::cout << name << "/" << ep->d_name << "/";
            char* new_name = strcat(name, "/");
            char* new_new_name = strcat(new_name, ep->d_name);
            ls(new_new_name);
        }
        else if (ep->d_type == DT_REG){
            std::cout << name << ep->d_name << std::endl;
        }
    }
    int close = closedir(dp);
}

int main(int argc, char** argv){
    if (argc <= 1){
        return 0;
    }
    else{
        for (int i = 1; i < argc; i++){
            ls(argv[i]);
        }
    }
    return 0;
}