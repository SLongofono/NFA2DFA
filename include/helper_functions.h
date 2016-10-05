#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

/* Include common header which is required in all files */
#include "common.h"

void 	print_dfa_table(struct list_head *head, state_t *final_states);
void 	print_final_states(struct list_head *dfa);
void 	print_state(state_t *state);
int 	check_move_final_state(state_t *in_states, state_t *nfa_final_states);

#endif /* __HELPER_FUNCTIONS_H__ */
