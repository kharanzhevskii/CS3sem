#include <iostream>
#include <string.h>


int main(int argc, char **argv) {
    if (argc<=1) {std::cout << "\n"; return 0;}
    if (strcmp(argv[1], "echo") != 0) {return 0;}
    if (argc==2) {std::cout << "\n"; return 0;}
    bool help = 0;
    bool crop = 0;
    if ((strcmp(argv[2], "--help") == 0) && argc==3) help = 1;
    if ((strcmp(argv[2], "-n")) == 0) crop = 1;
    if (help){
        std::cout << "write something and i will write it back\nuse -n to not add a newline\n";
        return 0;
    }
    int i = 2;
    if (crop) i = 3;
    for (i; i < argc-1; i++){
        std::cout << argv[i] << " ";
    }
    std::cout << argv[argc-1];
    if (!(crop)) std::cout << "\n";
    return 0;
    // std::cout << argc << " "<< argv[0][0];
    // return 0;
}
