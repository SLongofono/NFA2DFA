#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

/* Include common header which is required in all files */
#include "common.h"

void print_dfa_table(dfa_entry_t *head);
void print_final_states(dfa_entry_t *dfa);
void print_state(state_t *state);

#endif /* __HELPER_FUNCTIONS_H__ */