#ifndef CAPTURE_FORMATS_H
#define CAPTURE_FORMATS_H

#define MAX_PXFMTS 64

int capture_formats_get_pxfmt();
int capture_formats_set_pxfmt(int pixelformat);

char *capture_formats_get_pxfmt_name(int index);
void capture_formats_get_pxfmts(int all_pxfmts[MAX_PXFMTS], int *n_pxfmts);

#endif
