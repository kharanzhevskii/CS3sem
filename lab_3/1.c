#include <unistd.h>
#include <stdio.h>

int main() {
	printf("hello");
	size_t count = 5;
	int fd = 1;
	const char* p = "world";
	write(fd, p, count);
	return 0;
}
