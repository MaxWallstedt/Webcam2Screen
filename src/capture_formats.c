#include <linux/videodev2.h>
#include <stdio.h>
#include <string.h>
#include "capture_devices.h"
#include "capture_formats.h"
#include "xioctl.h"

static int get_current_format(struct v4l2_format *fmt)
{
	int fd;

	fd = capture_devices_get_device_fd();
	memset(fmt, 0, sizeof(struct v4l2_format));
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (xioctl(fd, (int)VIDIOC_G_FMT, fmt) == -1) {
		return -1;
	}

	return 0;
}

static int set_current_format(struct v4l2_format *fmt)
{
	int fd;

	fd = capture_devices_get_device_fd();

	if (xioctl(fd, (int)VIDIOC_S_FMT, fmt) == -1) {
		return -1;
	}

	return 0;
}

int capture_formats_get_pxfmt()
{
	struct v4l2_format fmt;

	if (get_current_format(&fmt) == -1) {
		return -1;
	}

	return fmt.fmt.pix.pixelformat;
}

int capture_formats_set_pxfmt(int pixelformat)
{
	struct v4l2_format fmt;

	if (get_current_format(&fmt) == -1) {
		return -1;
	}

	fmt.fmt.pix.pixelformat = pixelformat;

	if (set_current_format(&fmt) == -1) {
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

int  capture_formats_get_width()
{
	struct v4l2_format fmt;

	if (get_current_format(&fmt) == -1) {
		return -1;
	}

	return fmt.fmt.pix.width;
}

int  capture_formats_get_height()
{
	struct v4l2_format fmt;

	if (get_current_format(&fmt) == -1) {
		return -1;
	}

	return fmt.fmt.pix.height;
}

int capture_formats_set_frmsiz(int width, int height)
{
	struct v4l2_format fmt;

	if (get_current_format(&fmt) == -1) {
		return -1;
	}

	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;

	if (set_current_format(&fmt) == -1) {
		return -1;
	}

	return 0;
}

char *capture_formats_get_frmsiz_name(int index)
{
	int fd;
	struct v4l2_frmsizeenum frmsiz;
	char frmsizstr[1024];

	fd = capture_devices_get_device_fd();
	memset(&frmsiz, 0, sizeof(struct v4l2_frmsizeenum));
	frmsiz.index = index;
	frmsiz.pixel_format = capture_formats_get_pxfmt();

	if (xioctl(fd, (int)VIDIOC_ENUM_FRAMESIZES, &frmsiz) == -1) {
		return NULL;
	}

	switch (frmsiz.type) {
	case V4L2_FRMSIZE_TYPE_DISCRETE:
		sprintf(frmsizstr, "%ux%u",
		        frmsiz.discrete.width, frmsiz.discrete.height);
		break;
	default:
		sprintf(frmsizstr, "min: %ux%u, step: %ux%u, max: %ux%u",
		        frmsiz.stepwise.min_width,
		        frmsiz.stepwise.min_height,
		        frmsiz.stepwise.step_width,
		        frmsiz.stepwise.step_height,
		        frmsiz.stepwise.max_width,
		        frmsiz.stepwise.max_height);
	}

	return strdup(frmsizstr);
}

void capture_formats_get_frmsizs(int all_widths[MAX_FRMSIZS],
                                 int all_heights[MAX_FRMSIZS],
                                 int *n_frmsizs)
{
	int fd;
	int index;
	struct v4l2_frmsizeenum frmsiz;

	fd = capture_devices_get_device_fd();
	*n_frmsizs = 0;

	for (index = 0; index < MAX_FRMSIZS; ++index) {
		memset(&frmsiz, 0, sizeof(struct v4l2_frmsizeenum));
		frmsiz.index = index;
		frmsiz.pixel_format = capture_formats_get_pxfmt();

		if (xioctl(fd, (int)VIDIOC_ENUM_FRAMESIZES, &frmsiz)
		    == -1) {
			break;
		}

		switch (frmsiz.type) {
		case V4L2_FRMSIZE_TYPE_DISCRETE:
			all_widths[*n_frmsizs] = frmsiz.discrete.width;
			all_heights[*n_frmsizs] = frmsiz.discrete.height;
			break;
		default:
			all_widths[*n_frmsizs] = frmsiz.stepwise.max_width;
			all_heights[*n_frmsizs] =
				frmsiz.stepwise.max_height;
		}

		++*n_frmsizs;
	}
}
