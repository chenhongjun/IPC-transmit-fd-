#include <sys/socket.h>
#include <sys/param.h>
#include <stdlib.h>
//#include <sys/un.h>

ssize_t write_fd(int fd, void* ptr, size_t nbytes, int sendfd);
ssize_t read_fd(int fd, void* ptr, size_t nbytes, int* recvfd);
