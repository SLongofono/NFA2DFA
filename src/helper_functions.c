/*
 * Course		: EECS665
 * Name			: Waqar Ali
 * ID			: 2873101
 *
 * File
 * 	helper_functions.c
 *
 * Description
 * 	Source file which contains implementation of helper
 * 	funcitons which aid during different stages of NFA
 * 	to DFA conversion algorithm
 */

/* Include local headers */
#include "helper_functions.h"
 
 /*
 * print_dfa_table
 * Convenience function for printing the DFA transition table
 */
void print_dfa_table(struct list_head *head, state_t *final_states)
{
	dfa_entry_t 	*entry = list_entry(head->next, dfa_entry_t, list);
	dfa_move_t	*tmp;
	char 		depth = 'a';

	/* Print table header */
	printf("Initial State: {%d}\n", entry->id);

	printf("Final States: {");
	print_state(final_states);
	printf("}\n");

	/* Print table symbols */
	printf("State	");
	list_for_each_entry(tmp, &(entry->dfa_transitions), list) {
		printf("%c	", depth);
		depth++;
	}

	/* Print a new line */
	printf("\n");

	/* Iterate over dfa states */
	list_for_each_entry(entry, head, list) {
		printf("%d	", entry->id);

		/* Iterate over dfa moves */
		dfa_move_t *move;

		list_for_each_entry(move, &(entry->dfa_transitions), list) {
			if (move->id != 0) {
				printf("{%d}	", move->id);
			} else {
				printf("{}	");
			}
		}

		/* Output a new line */
		printf("\n");
	}

	/* All done here */
	return;
}

/*
 * print_final_states
 * Convenience function for finding and printing final states of a dfa
 */
void print_final_states(struct list_head *dfa)
{
	dfa_entry_t 	*node = list_entry(dfa->prev, dfa_entry_t, list);
	int 		final_state_id = node->id;
	int 		count = 0;

	list_for_each_entry(node, dfa, list) {
		/* Iterate over the list of moves */
		dfa_move_t *move;

		list_for_each_entry(move, &(node->dfa_transitions), list) {
			if (move->id == final_state_id) {
				if (count != 0) printf (",");
				printf ("%d", node->id);
				count++;
				break;
			}
		}
	}

	/* Task Complete */
	return;
}

/*
 * check_move_final_state
 * Convenience function for checking if a move of DFA state contains
 * one of the NFA's final states. Returns boolean
 */
int check_move_final_state(state_t *in_states, state_t *nfa_final_states)
{
	int 		result = 0;
	state_t		*nfa_iter = nfa_final_states;
	state_t		*in_iter = in_states;

	/* Iterate over list of input states */
	do {
		/* Break the loop if input state list is empty */
		if (in_iter->id == -1) break;

		/* Iterate over list of final NFA states */
		do {
			if (in_iter->id == nfa_iter->id) {
				/* NFA final state found in the move list of dfa */
				return 1;
			}

			/* Proceed to the next NFA state */
			nfa_iter = nfa_iter->next;
		} while (nfa_iter != nfa_final_states);

		/* Proceed to the next input state */
		in_iter = in_iter->next;
	} while (in_iter != in_states);


	/* Return the result to caller */
	return result;
}

/*
 * print_state
 * Convenience function for printing a state list
 */
void print_state(state_t *state)
{
	state_t *head = state;
	state_t *iter = head;
	int count = 0;
	int threshold = 0;
	int print_val;

	do {
		state_t *temp_iter = state;

		/* Reset print value i.e. max value of 32-bit signed integer */
		print_val = 0x7FFFFFFF;

		if (temp_iter->id != -1) {
			do {
				if (temp_iter->id > threshold && temp_iter->id < print_val) {
					print_val = temp_iter->id;
				}
				temp_iter = temp_iter->next;
			} while (temp_iter != head);
		} else {
			break;
		}

		/* Update the threshold value for the next iteration */
		threshold = print_val;

		/* Print the value obained */
		if (count != 0) printf(",");
		printf("%d", threshold);

		/* Proceed to next state in the list */
		iter = iter->next;
		count++;
	} while (iter != head);

	/* Done */
	return;
}
