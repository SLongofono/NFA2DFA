#ifndef __NFA2DFA_H__
#define __NFA2DFA_H__

/* Include required local headers */
#include "helper_functions.h"

/* Primary Functions for NFA2DFA Conversion */
void	e_closure(state_t *input_states, struct list_head *transitions, int empty, state_t **output_states);
void 	mark(state_t *input_states, struct list_head *transitions, struct list_head *moves);
int	state_not_marked(state_t *state, struct list_head *state_list, int *state_id);

#endif /* __NFA2DFA_H__ */
