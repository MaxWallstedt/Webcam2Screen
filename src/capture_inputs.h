#ifndef CAPTURE_INPUTS_H
#define CAPTURE_INPUTS_H

#define MAX_INPUTS 32

int capture_inputs_get_input();
int capture_inputs_set_input(int input);

char *capture_inputs_get_input_name(int input);
void capture_inputs_get_inputs(int all_inputs[MAX_INPUTS],
                               int *n_inputs);

#endif
