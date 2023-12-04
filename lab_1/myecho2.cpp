#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char **argv) {
    if (!(strcmp("echo", argv[0]))) return 0;
    if (argc == 1) {cout << endl; return 0;}
    bool long_flag = 0;
    bool no_end = 0;
    if (!strcmp(argv[1], "--help")) long_flag = 1;
    if (!strcmp(argv[1], "-n")) no_end = 1;
    if (!strcmp(argv[2], "--help")) long_flag = 1;
    if (!strcmp(argv[2], "-n")) no_end = 1;
    if (argc==2 && no_end) return 0;
    if (argc==2 && long_flag){
        cout << "say something, and i will say it back\nadd -n to nott add a newline\n";
    }
    for (int i = 1; i < argc-1; i++){
        cout << argv[i] << " ";
    }
    cout << argv[argc-1];
    if (!(no_end)) cout << endl;
    return 0;
}
