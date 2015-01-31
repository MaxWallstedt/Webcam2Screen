#!/bin/bash

CC="gcc"
CFLAGS="-pedantic -Wall -Wextra -Werror `pkg-config --cflags gtk+-3.0`"
LIBS="`pkg-config --libs gtk+-3.0`"
TARGET="Webcam2Screen"
SRC=(
	"src/main.c"
	"src/device_list.c"
	"src/input_list.c"
	"src/xioctl.c"
	"src/capture_devices.c"
	"src/capture_inputs.c"
)

$CC $CFLAGS -o $TARGET ${SRC[@]} $LIBS
