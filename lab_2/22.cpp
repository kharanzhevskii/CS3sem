#include <iostream>
int x(const char* xx){
	return 1;
}
void b(const char* xx){
        x(xx);
}
void f(const char* xx){
	b(xx);
}
int main(){
	const char* aaa = "helo world";
	f(aaa);
	return 0;
}

