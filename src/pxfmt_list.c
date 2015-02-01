#include <gtk/gtk.h>
#include <stdlib.h>
#include "capture_formats.h"
#include "frmsiz_list.h"
#include "main.h"

static gint all_pxfmts[MAX_PXFMTS];
static gint n_pxfmts;
static gint cur_pxfmt;

static GtkComboBoxText *pxfmt_list;

void pxfmt_list_reload_pxfmts()
{
	gint i;
	gchar *pxfmtstr;

	gtk_combo_box_text_remove_all(pxfmt_list);
	capture_formats_get_pxfmts(all_pxfmts, &n_pxfmts);

	for (i = 0; i < n_pxfmts; ++i) {
		pxfmtstr = capture_formats_get_pxfmt_name(i);
		gtk_combo_box_text_append_text(pxfmt_list, pxfmtstr);
		free(pxfmtstr);

		if (all_pxfmts[i] == capture_formats_get_pxfmt()) {
			cur_pxfmt = i;
			gtk_combo_box_set_active(GTK_COMBO_BOX(pxfmt_list),
			                         i);
		}
	}

	if (n_pxfmts == 0) {
		gtk_combo_box_text_append_text(pxfmt_list,
		                               "No pixel formats");
		gtk_combo_box_set_active(GTK_COMBO_BOX(pxfmt_list), 0);
		cur_pxfmt = 0;
	}
}

static void changed_cb(GtkComboBox *widget,
                       gpointer user_data __attribute__((unused)))
{
	int new_pxfmt;

	new_pxfmt = gtk_combo_box_get_active(widget);

	if (new_pxfmt != cur_pxfmt) {
		cur_pxfmt = new_pxfmt;
		capture_formats_set_pxfmt(all_pxfmts[cur_pxfmt]);
		frmsiz_list_reload_frmsizs();
	}
}

void pxfmt_list_init()
{
	GtkBuilder *builder;
	GObject *gobject;

	builder = main_get_builder();
	gobject = gtk_builder_get_object(builder, "pxfmtList");
	pxfmt_list = GTK_COMBO_BOX_TEXT(gobject);

	g_signal_connect(gobject, "changed", G_CALLBACK(changed_cb), NULL);
	pxfmt_list_reload_pxfmts();
}
