#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

//ssize_t read(int fd, void *buf, size_t count);

ssize_t mywrite(size_t count, char* buf){
	ssize_t ret = 0;
	ssize_t written = write(1, buf, count);
	ret += written;
	if (written < 0){
		perror("idk");
		return -1;
	}
	while (written < count){
		count -= written;
		buf += written;
		ssize_t written = write(1, buf, count);
		ret += written;
	}
	return ret;
}

int main(){
	char* buf = (char*) malloc(40);
	ssize_t num = read(0, buf, 40);
	while (num < 40){
		if (num == 0) break;
		if (num < 0){
		       	perror("idk");
			return 1;
		}
		ssize_t written = mywrite(num, buf);
		num -= written;
	}
	free(buf);
	return 0;
}	
