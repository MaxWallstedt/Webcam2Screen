#include <gtk/gtk.h>
#include <stdlib.h>
#include "capture_formats.h"
#include "main.h"

static gint all_widths[MAX_FRMSIZS];
static gint all_heights[MAX_FRMSIZS];
static gint n_frmsizs;
static gint cur_frmsiz;

static GtkComboBoxText *frmsiz_list;

void frmsiz_list_reload_frmsizs()
{
	gint i;
	gchar *frmsizstr;

	gtk_combo_box_text_remove_all(frmsiz_list);
	capture_formats_get_frmsizs(all_widths, all_heights, &n_frmsizs);

	for (i = 0; i < n_frmsizs; ++i) {
		frmsizstr = capture_formats_get_frmsiz_name(i);
		gtk_combo_box_text_append_text(frmsiz_list, frmsizstr);
		free(frmsizstr);

		if (all_widths[i] == capture_formats_get_width()
		    && all_heights[i] == capture_formats_get_height()) {
			cur_frmsiz = i;
			gtk_combo_box_set_active(GTK_COMBO_BOX(frmsiz_list),
			                         i);
		}
	}

	if (n_frmsizs == 0) {
		gtk_combo_box_text_append_text(frmsiz_list,
		                               "No frame sizes");
		gtk_combo_box_set_active(GTK_COMBO_BOX(frmsiz_list), 0);
		cur_frmsiz = 0;
	}
}

static void changed_cb(GtkComboBox *widget,
                       gpointer user_data __attribute__((unused)))
{
	int new_frmsiz;

	new_frmsiz = gtk_combo_box_get_active(widget);

	if (new_frmsiz != cur_frmsiz) {
		cur_frmsiz = new_frmsiz;
		capture_formats_set_frmsiz(all_widths[cur_frmsiz],
		                           all_heights[cur_frmsiz]);
	}
}

void frmsiz_list_init()
{
	GtkBuilder *builder;
	GObject *gobject;

	builder = main_get_builder();
	gobject = gtk_builder_get_object(builder, "frmsizList");
	frmsiz_list = GTK_COMBO_BOX_TEXT(gobject);

	g_signal_connect(gobject, "changed", G_CALLBACK(changed_cb), NULL);
	frmsiz_list_reload_frmsizs();
}
