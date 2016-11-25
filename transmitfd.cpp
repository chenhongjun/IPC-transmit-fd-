#include "transmitfd.h"

ssize_t write_fd(int fd, void* ptr, size_t nbytes, int sendfd)
{
	struct msghdr msg;
	struct iovec iov[1];
//初始化cmsghdr
//#ifdef HAVE_MSGHDR_MSG_CONTROL
	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;

	struct cmsghdr* cmptr;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);

	cmptr = CMSG_FIRSTHDR(&msg);

	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;

	*((int*) CMSG_DATA(cmptr)) = sendfd;//辅助数据
//#else
//	msg.msg_accrights = (caddr_t)&sendfd;
//	msg.msg_accrightslen = sizeof(int);
//#endif

	msg.msg_name = NULL;//初始化msghdr
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;

	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	return (sendmsg(fd, &msg, 0));
}

ssize_t read_fd(int fd, void* ptr, size_t nbytes, int* recvfd)
{
	struct msghdr msg;
	struct iovec iov[1];
	ssize_t n;
//#ifdef HAVE_MSGHDR_MSG_CONTROL
	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;
	struct cmsghdr* cmptr;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
//#else
//	int newfd;
//	msg.msg_accrights = (caddr_t)&newfd;
//	msg.msg_accrightslen = sizeof(int);
//#endif
	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;

	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	
	if ((n = recvmsg(fd, &msg, 0)) <= 0)
		return 0;

//#ifdef HAVE_MSGHDR_MSG_CONTROL
	if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL && cmptr->cmsg_len == CMSG_LEN(sizeof(int)))
	{
		if (cmptr->cmsg_level != SOL_SOCKET)
			exit(-1);
		if (cmptr->cmsg_type != SCM_RIGHTS)
			exit(-1);
		*recvfd = *((int*) CMSG_DATA(cmptr));
	}
	else
	  *recvfd = -1;
//#endif

	return n;
}
