#include <sys/ioctl.h>
#include <errno.h>

int xioctl(int fd, int request, void *argp)
{
	int r;

	do {
		r = ioctl(fd, request, argp);
	} while (r == -1 && errno == EINTR);

	return r;
}
