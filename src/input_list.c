#include <gtk/gtk.h>
#include <stdlib.h>
#include "capture_inputs.h"
#include "pxfmt_list.h"
#include "main.h"

static gint all_inputs[MAX_INPUTS];
static gint n_inputs;
static gint cur_inp;

static GtkComboBoxText *input_list;

void input_list_reload_inputs()
{
	gint i;
	gchar *inpstr;

	gtk_combo_box_text_remove_all(input_list);
	capture_inputs_get_inputs(all_inputs, &n_inputs);

	for (i = 0; i < n_inputs; ++i) {
		inpstr = capture_inputs_get_input_name(all_inputs[i]);
		gtk_combo_box_text_append_text(input_list, inpstr);
		free(inpstr);

		if (all_inputs[i] == capture_inputs_get_input()) {
			cur_inp = i;
			gtk_combo_box_set_active(GTK_COMBO_BOX(input_list),
			                         i);
		}
	}

	if (n_inputs == 0) {
		gtk_combo_box_text_append_text(input_list, "No inputs");
		gtk_combo_box_set_active(GTK_COMBO_BOX(input_list), 0);
		cur_inp = 0;
	}
}

static void changed_cb(GtkComboBox *widget,
                       gpointer user_data __attribute__((unused)))
{
	int new_inp;

	new_inp = gtk_combo_box_get_active(widget);

	if (new_inp != cur_inp) {
		cur_inp = new_inp;
		capture_inputs_set_input(all_inputs[cur_inp]);
		pxfmt_list_reload_pxfmts();
	}
}

void input_list_init()
{
	GtkBuilder *builder;
	GObject *gobject;

	builder = main_get_builder();
	gobject = gtk_builder_get_object(builder, "inputList");
	input_list = GTK_COMBO_BOX_TEXT(gobject);

	g_signal_connect(gobject, "changed", G_CALLBACK(changed_cb), NULL);
	input_list_reload_inputs();
}
