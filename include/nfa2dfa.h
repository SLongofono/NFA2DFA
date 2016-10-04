#ifndef __NFA2DFA_H__
#define __NFA2DFA_H__

/* Include required local headers */
#include "helper_functions.h"

/* Primary Functions for NFA2DFA Conversion */
void e_closure(state_t *input_states, transitions_t **transitions, int empty, state_t **output_states);
void mark(state_t *input_states, transitions_t **transitions, state_t **a_states, state_t **b_states);
int state_not_marked(state_t *state, state_list_t *list_head);

#endif /* __NFA2DFA_H__ */
