#include <linux/videodev2.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "capture_devices.h"
#include "capture_devices_filename.h"
#include "xioctl.h"

static int cur_fd = -1;
static int cur_dev = -1;

int capture_devices_get_device()
{
	return cur_dev;
}

int capture_devices_get_device_fd()
{
	return cur_fd;
}

void capture_devices_close_device_fd()
{
	if (cur_fd != -1) {
		close(cur_fd);
		cur_fd = -1;
		cur_dev = -1;
	}
}

int capture_devices_set_device(int device)
{
	int nfd;

	if (device != cur_dev) {
		nfd = open(capture_devices_filename[device], O_RDWR);

		if (nfd == -1) {
			return -1;
		}

		capture_devices_close_device_fd();
		cur_fd = nfd;
		cur_dev = device;
	}

	return 0;
}

char *capture_devices_get_device_name(int device)
{
	struct v4l2_capability cap;
	int fd;
	char *filename;

	memset(&cap, 0, sizeof(struct v4l2_capability));

	if (device != cur_dev) {
		filename = capture_devices_filename[device];

		if ((fd = open(filename, O_RDWR)) == -1) {
			return NULL;
		}
	} else {
		fd = cur_fd;
	}

	if (xioctl(fd, (int)VIDIOC_QUERYCAP, &cap) == -1) {
		if (fd != cur_fd) {
			close(fd);
		}
		return NULL;
	}

	if (fd != cur_fd) {
		close(fd);
	}

	return strdup((char *)cap.card);
}

static int check_capture_capability(int fd)
{
	struct v4l2_capability cap;

	memset(&cap, 0, sizeof(struct v4l2_capability));

	if (xioctl(fd, (int)VIDIOC_QUERYCAP, &cap) == -1) {
		return -1;
	}

	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
		return -1;
	}

	if ((cap.capabilities & V4L2_CAP_DEVICE_CAPS) != 0
	    && (cap.device_caps & V4L2_CAP_VIDEO_CAPTURE) == 0) {
		return -1;
	}

	return 0;
}

void capture_devices_get_devices(int all_devices[MAX_DEVICES],
                                 int *n_devices)
{
	int device;
	int fd;
	char *filename;
	int prev_dev;

	capture_devices_close_device_fd();
	prev_dev = cur_dev;
	cur_dev = -1;
	*n_devices = 0;

	for (device = 0; device < MAX_DEVICES; ++device) {
		filename = capture_devices_filename[device];

		if ((fd = open(filename, O_RDWR)) == -1) {
			continue;
		}

		if (check_capture_capability(fd) == -1) {
			close(fd);
			continue;
		}

		all_devices[*n_devices] = device;
		++*n_devices;
		close(fd);

		if (device == prev_dev) {
			capture_devices_set_device(device);
		}
	}

	if (cur_fd == -1) {
		capture_devices_set_device(0);
	}
}
