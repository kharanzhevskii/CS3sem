#include <stdio.h>
#include <stdbool.h>
int main(){
    char q = 'q';
    bool symbl = (q >> 7) && 1;
    printf("%d", symbl);
    return 0;
}