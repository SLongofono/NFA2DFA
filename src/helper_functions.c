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
	dfa_entry_t *entry = list_entry(head->next, dfa_entry_t, list);
	char buf_a[32];
	char buf_b[32];

	/* Print table header */
	printf("Initial State: {%d}\n", entry->id);

	printf("Final States: {");
	print_final_states(head);
	printf("}\n");

	printf("State	a	b\n");

	/* Iterate through dfa transitions and print them */
	list_for_each_entry(entry, head, list) {
		if (entry->trans_a == 0) {
			sprintf(buf_a, "");
		} else {
			sprintf(buf_a, "%d", entry->trans_a);
		}

		if (entry->trans_b == 0) {
			sprintf(buf_b, "");
		} else {
			sprintf(buf_b, "%d", entry->trans_b);
		}

		printf("%d	{%s}	{%s}\n", entry->id, buf_a, buf_b);
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
	dfa_entry_t *node = list_entry(dfa->prev, dfa_entry_t, list);
	int final_state_id = node->id;
	int count = 0;

	list_for_each_entry(node, dfa, list) {
		if (node->trans_a == final_state_id || node->trans_b == final_state_id) {
			if (count != 0) printf (",");
			printf ("%d", node->id);
			count++;
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

	do {
		if (iter->id != -1) {
			if (count != 0) printf(",");
			printf("%d", iter->id);
		} else {
			break;
		}

		/* Proceed to next state in the list */
		iter = iter->next;
		count++;
	} while (iter != head);

	/* Done */
	return;
}
