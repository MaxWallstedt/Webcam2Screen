#include <linux/videodev2.h>
#include <string.h>
#include "capture_devices.h"
#include "capture_inputs.h"
#include "xioctl.h"

int capture_inputs_get_input()
{
	int input;
	int fd;

	fd = capture_devices_get_device_fd();

	if (xioctl(fd, (int)VIDIOC_G_INPUT, &input) == -1) {
		return -1;
	}

	return input;
}

int capture_inputs_set_input(int input)
{
	int fd;

	fd = capture_devices_get_device_fd();

	if (xioctl(fd, (int)VIDIOC_S_INPUT, &input) == -1) {
		return -1;
	}

	return 0;
}

char *capture_inputs_get_input_name(int input)
{
	int fd;
	struct v4l2_input inp;

	fd = capture_devices_get_device_fd();
	memset(&inp, 0, sizeof(struct v4l2_input));
	inp.index = input;

	if (xioctl(fd, (int)VIDIOC_ENUMINPUT, &inp) == -1) {
		return NULL;
	}

	return strdup((char *)inp.name);
}

void capture_inputs_get_inputs(int all_inputs[MAX_INPUTS],
                               int *n_inputs)
{
	int fd;
	int index;
	struct v4l2_input inp;

	*n_inputs = 0;
	fd = capture_devices_get_device_fd();

	for (index = 0; index < MAX_INPUTS; ++index) {
		memset(&inp, 0, sizeof(struct v4l2_input));
		inp.index = index;

		if (xioctl(fd, (int)VIDIOC_ENUMINPUT, &inp) == -1) {
			break;
		}

		if (inp.type != V4L2_INPUT_TYPE_CAMERA) {
			continue;
		}

		all_inputs[*n_inputs] = index;
		++*n_inputs;
	}
}
