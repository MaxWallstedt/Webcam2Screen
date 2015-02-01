#ifndef CAPTURE_DEVICES_H
#define CAPTURE_DEVICES_H

#define MAX_DEVICES 64

int capture_devices_get_device();
int capture_devices_get_device_fd();
void capture_devices_close_device_fd();
int capture_devices_set_device(int device);

char *capture_devices_get_device_name(int device);
void capture_devices_get_devices(int all_devices[MAX_DEVICES],
                                 int *n_devices);

#endif
