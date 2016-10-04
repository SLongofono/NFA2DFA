#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

/* Include common header which is required in all files */
#include "common.h"

void print_dfa_table(struct list_head *head);
void print_final_states(struct list_head *dfa);
void print_state(state_t *state);

#endif /* __HELPER_FUNCTIONS_H__ */
