#include <gtk/gtk.h>
#include "device_list.h"
#include "input_list.h"
#include "capture_devices.h"

static GtkBuilder *builder;

GtkBuilder *main_get_builder()
{
	return builder;
}

int main(int argc, char *argv[])
{
	GObject *object;

	gtk_init(&argc, &argv);

	/* Read layout file */
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "builder.ui", NULL);

	/* Top level window */
	object = gtk_builder_get_object(builder, "window");
	g_signal_connect(object, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	/* Combo boxes */
	device_list_init();
	input_list_init();

	gtk_main();

	capture_devices_close_device_fd();

	return 0;
}
