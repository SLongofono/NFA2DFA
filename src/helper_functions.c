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
void print_dfa_table(struct list_head *head)
{
	dfa_entry_t 	*entry = list_entry(head->next, dfa_entry_t, list);
	dfa_move_t	*tmp;
	char 		depth = 'a';

	/* Print table header */
	printf("Initial State: {%d}\n", entry->id);

	printf("Final States: {");
	print_final_states(head);
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

		/* Reset print value */
		print_val = 1000;

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
