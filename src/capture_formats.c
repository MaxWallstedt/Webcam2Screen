#include <linux/videodev2.h>
#include <string.h>
#include "capture_devices.h"
#include "capture_formats.h"
#include "xioctl.h"

int capture_formats_get_pxfmt()
{
	int fd;
	struct v4l2_format fmt;

	fd = capture_devices_get_device_fd();
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (xioctl(fd, (int)VIDIOC_G_FMT, &fmt) == -1) {
		return -1;
	}

	return fmt.fmt.pix.pixelformat;
}

int capture_formats_set_pxfmt(int pixelformat)
{
	int fd;
	struct v4l2_format fmt;

	fd = capture_devices_get_device_fd();
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (xioctl(fd, (int)VIDIOC_G_FMT, &fmt) == -1) {
		return -1;
	}

	fmt.fmt.pix.pixelformat = pixelformat;

	if (xioctl(fd, (int)VIDIOC_S_FMT, &fmt) == -1) {
		return -1;
	}

	return 0;
}

char *capture_formats_get_pxfmt_name(int index)
{
	int fd;
	struct v4l2_fmtdesc fmt;
	char pxfmtstr[56];

	fd = capture_devices_get_device_fd();
	memset(&fmt, 0, sizeof(struct v4l2_fmtdesc));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.index = index;

	if (xioctl(fd, (int)VIDIOC_ENUM_FMT, &fmt) == -1) {
		return NULL;
	}

	strcpy(pxfmtstr, (char *)fmt.description);

	if ((fmt.flags & V4L2_FMT_FLAG_COMPRESSED) != 0) {
		strcat(pxfmtstr, " (Compressed)");
	}

	if ((fmt.flags & V4L2_FMT_FLAG_EMULATED) != 0) {
		strcat(pxfmtstr, " (Emulated)");
	}

	return strdup(pxfmtstr);
}

void capture_formats_get_pxfmts(int all_pxfmts[MAX_PXFMTS], int *n_pxfmts)
{
	int fd;
	int index;
	struct v4l2_fmtdesc fmt;

	*n_pxfmts = 0;
	fd = capture_devices_get_device_fd();

	for (index = 0; index < MAX_PXFMTS; ++index) {
		memset(&fmt, 0, sizeof(struct v4l2_fmtdesc));
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.index = index;

		if (xioctl(fd, (int)VIDIOC_ENUM_FMT, &fmt) == -1) {
			break;
		}

		all_pxfmts[*n_pxfmts] = fmt.pixelformat;
		++*n_pxfmts;
	}
}
