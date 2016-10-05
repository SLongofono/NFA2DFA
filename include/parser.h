#ifndef __PARSER_H__
#define __PARSER_H__

/* Include required local headers */
#include "helper_functions.h"

void get_states(char *input_buffer, state_t *in_head, int rule_type, int *total_states);
void get_transitions(char *input_buffer, struct list_head *state_list);

#endif /* __PARSER_H__ */
