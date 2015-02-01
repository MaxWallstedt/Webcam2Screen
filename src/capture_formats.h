#ifndef CAPTURE_FORMATS_H
#define CAPTURE_FORMATS_H

#define MAX_PXFMTS 64

int capture_formats_get_pxfmt();
int capture_formats_set_pxfmt(int pixelformat);

char *capture_formats_get_pxfmt_name(int index);
void capture_formats_get_pxfmts(int all_pxfmts[MAX_PXFMTS], int *n_pxfmts);

#define MAX_FRMSIZS 64

int capture_formats_get_width();
int capture_formats_get_height();
int capture_formats_set_frmsiz(int width, int height);

char *capture_formats_get_frmsiz_name(int index);
void capture_formats_get_frmsizs(int all_widths[MAX_FRMSIZS],
                                 int all_heights[MAX_FRMSIZS],
                                 int *n_frmsizs);

#endif
