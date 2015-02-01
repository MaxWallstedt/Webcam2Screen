#include <gtk/gtk.h>
#include <stdlib.h>
#include "capture_devices.h"
#include "input_list.h"
#include "main.h"

static gint all_devices[MAX_DEVICES];
static gint n_devices;
static gint cur_dev;

static GtkComboBoxText *device_list;

void device_list_reload_devices()
{
	gint i;
	gchar *devstr;

	gtk_combo_box_text_remove_all(device_list);
	capture_devices_get_devices(all_devices, &n_devices);

	for (i = 0; i < n_devices; ++i) {
		devstr = capture_devices_get_device_name(all_devices[i]);
		gtk_combo_box_text_append_text(device_list, devstr);
		free(devstr);

		if (all_devices[i] == capture_devices_get_device()) {
			gtk_combo_box_set_active(GTK_COMBO_BOX(device_list),
                                                 i);
		}
	}

	if (n_devices == 0) {
		gtk_combo_box_text_append_text(device_list, "No devices");
	}
}

static void popup_shown_cb(GObject *gobject,
                           GParamSpec *pspec __attribute__((unused)),
                           gpointer user_data __attribute__((unused)))
{
	gboolean popup_shown;

	g_object_get(gobject, "popup-shown", &popup_shown, NULL);

	if (popup_shown) {
		device_list_reload_devices();
	}
}

static void changed_cb(GtkComboBox *widget,
                       gpointer user_data __attribute__((unused)))
{
	gint new_dev;
	gint new_index;

	new_index = gtk_combo_box_get_active(widget);
	new_dev = all_devices[new_index];

	if (new_dev != cur_dev) {
		cur_dev = new_dev;
		capture_devices_set_device(all_devices[cur_dev]);
		input_list_reload_inputs();
	}
}

void device_list_init()
{
	GtkBuilder *builder;
	GObject *gobject;

	builder = main_get_builder();
	gobject = gtk_builder_get_object(builder, "deviceList");
	device_list = GTK_COMBO_BOX_TEXT(gobject);

	g_signal_connect(gobject, "notify::popup-shown",
	                 G_CALLBACK(popup_shown_cb), NULL);
	g_signal_connect(gobject, "changed", G_CALLBACK(changed_cb), NULL);
	capture_devices_set_device(0);
	device_list_reload_devices();
}
