/*
 * Course		: EECS665
 * Name			: Waqar Ali
 * ID			: 2873101
 *
 * File
 * 	nfa2dfa.c
 *
 * Description
 * 	Source file which contains implementation of primary
 * 	funcitons for NFA to DFA conversion algorithm
 */

/* Include local headers */
#include "nfa2dfa.h"

 /*
 * e_closure
 * Recursive function to calculate e-closure of a set of input states
 */
void e_closure(state_t *input_states, transitions_t **transitions, int empty, state_t **output_states)
{
	state_t	*state_iter = input_states;

	do {
		/* Check if the list of output states is empty */
		if (empty == 1) {
			/* Intialize the head element of output state linked list */
			(*output_states) = malloc(sizeof(struct state));
			list_init((*output_states));

			(*output_states)->id = state_iter->id;
		} else {
			/* Add this state to the output states linked list */
			state_t *new_state = malloc(sizeof(struct state));

			/* Copy the id-value */
			new_state->id = state_iter->id;

			/* Add the node to the list */
			list_add((*output_states), new_state);
		}

		/* Check the transition table for any E-transitions of the input state */
		state_t *head = transitions[state_iter->id - 1]->E;

		if (head->id != -1) {
			/* Calculate the e-closure of the E-transition linked list */
			e_closure(head, transitions, 0, output_states);
		}

		/* Proceed to the next state in input list */
		state_iter = state_iter->next;
	} while (state_iter != input_states);

	/* All done here */
	return;
}

/*
 * Mark
 * Function to mark the input states
 */
void mark(state_t *input_states, transitions_t **transitions, state_t **a_move, state_t **b_move)
{
	state_t	*state_iter = input_states;
	state_t *a_states, *b_states;

	/* Initialize a-states and b-states */
	a_states = malloc(sizeof(struct state));
	b_states = malloc(sizeof(struct state));

	*a_move = a_states;
	*b_move = b_states;

	list_init(a_states);
	list_init(b_states);

	/* Calculate move-a for input states */
	do {
		state_t *head = transitions[state_iter->id - 1]->a;
		state_t *node = head;

		if (node->id != -1) {
			do {
				if (a_states->id == -1) {
					a_states->id = node->id;
				} else {
					/* Create a new state node to be added to mova-a linked list */
					state_t *new_state = malloc(sizeof(struct state));

					/* Copy the state id value */
					new_state->id = node->id;

					/* Add the new state to the linked list */
					list_add(a_states, new_state);
				}

				/* Proceed to the next node */
				node = node->next;
			} while (node != head);
		}

		/* Proceed to the next input state */
		state_iter = state_iter->next;
	} while (state_iter != input_states);

	/* Calculate move-b for input states */
	do {
		state_t *head = transitions[state_iter->id - 1]->b;
		state_t *node = head;

		if (node->id != -1) {
			do {
				if (b_states->id == -1) {
					b_states->id = node->id;
				} else {
					/* Create a new state node to be added to mova-a linked list */
					state_t *new_state = malloc(sizeof(struct state));

					/* Copy the state id value */
					new_state->id = node->id;

					/* Add the new state to the linked list */
					list_add(b_states, new_state);
				}

				/* Proceed to the next node */
				node = node->next;
			} while (node != head);
		}

		/* Proceed to the next input state */
		state_iter = state_iter->next;
	} while (state_iter != input_states);

	return;
}

/* state_not_marked
 * Helper function for finding if a state has been marked
 */
int state_not_marked(state_t *state, state_list_t *list_head)
{
	int result = 1;
	int id_found = 0;
	state_t *state_iter = state;
	state_t *iter_1, *iter_2;
	state_list_t *list_iter = list_head;

	/* Iterate over linked list of linked lists of states */
	do {
		/* Get the first linked list of states */
		state_iter = list_iter->head;

		if (state_iter->size == state->size) {
			/* Iterate over the input linked list to see if it is equivalent
			 * to the linked list in marked states */
			iter_1 = state_iter;
			id_found = 0;

			do {
				iter_2 = state;
				do {
					if (iter_1->id == iter_2->id) {
						/* ID has been found */
						id_found++;

						/* Break the loop */
						break;
					}

					/* Proceed to the next state */
					iter_2 = iter_2->next;
				} while (iter_2 != state);

				/* We should have found the id by now */
				if (id_found == state->size) {
					/* State is already marked */
					return 0;
				}

				/* Proceed to the next state */
				iter_1 = iter_1->next;
			} while (iter_1 != state_iter);
		} 

		/* Proceed to the next linked list */
		list_iter = list_iter->next;
	} while (list_iter != list_head);

	return result;
}
