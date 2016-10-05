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
void e_closure(state_t *input_states, struct list_head *transitions, int empty, state_t **output_states)
{
	state_t *input_iter = input_states;

	do {
		/* Check if the list of output states is empty */
		if (empty == 1) {
			/* Intialize the head element of output state linked list */
			(*output_states) = malloc(sizeof(struct state));
			list_init((*output_states));

			(*output_states)->id = input_iter->id;
		} else {
			state_t *temp_state = *output_states;
			int found = 0;

			do {
				if (temp_state->id == input_iter->id) {
					found = 1;
					break;
				}

				/* Proceed to the next node */
				temp_state = temp_state->next;
			} while (temp_state != *output_states);

			if (found == 0) {
				/* Add this state to the output states linked list */
				state_t *new_state = malloc(sizeof(struct state));

				/* Copy the id-value */
				new_state->id = input_iter->id;

				/* Add the node to the list */
				list_add((*output_states), new_state);
			}
		}

		/* Check the transition table for any E-transitions of the input state */
		state_list_t *node = list_entry((transitions[input_iter->id - 1]).prev, state_list_t, list);
		state_t *state_iter = node->state_ptr;

		do {
			if (state_iter->id != -1) {
				/* Make sure that the state has not already been added */
				state_t *temp_state = *output_states;
				int	found = 0;

				do {
					if (temp_state->id == state_iter->id) {
						found = 1;
						break;
					}

					/* Proceed to next state in the output list */
					temp_state = temp_state->next;
				} while (temp_state != *output_states);

				if (found == 0) {
					/* Calculate the e-closure of the E-transition linked list */
					e_closure(state_iter, transitions, 0, output_states);
				}
			}

			/* Proceed to the next state in input list */
			state_iter = state_iter->next;
		} while (state_iter != node->state_ptr);

		/* Move on to the next state in the input list */
		input_iter = input_iter->next;
	} while (input_iter != input_states);

	/* All done here */
	return;
}

/*
 * Mark
 * Function to mark the input states
 */
void mark(state_t *input_states, struct list_head *transitions, struct list_head *moves_list)
{
	state_list_t	*transition;
	state_t		*state_iter = input_states;
	char		id = 'a';

	/* Iterate over the list of transitions */
	list_for_each_entry(transition, &transitions[0], list) {
		if (&(transition->list) == (&(transitions[0]))->prev) break;

		/* Create a state-list to be returned */
		state_list_t *moves = malloc(sizeof(struct state_list));
		state_list_init(moves);

		/* Add the list node to moves list */
		list_add_tail(&(moves->list), moves_list);

		/* Create a new state list */
		moves->state_ptr = malloc(sizeof(struct state));

		/* Initialize the empty state */
		list_init(moves->state_ptr);


		do {
			state_list_t	*node;
			char		depth = 'a';
			list_for_each_entry(node, &transitions[state_iter->id -1], list) {

				if (depth == id) {
					/* Copy the move value to state list */
					state_t	*new_state;
					state_t *state_iter = node->state_ptr;

					do {
						if (state_iter->id != -1) {
							/* Make sure that the state we are adding has not been
							   added already */
							state_t	*temp_state = moves->state_ptr;
							int 	found = 0;

							do {
								if (temp_state->id == state_iter->id) {
									found = 1;
									break;
								}

								/* Proceed to the next state in the linked list */
								temp_state = temp_state->next;
							} while (temp_state != moves->state_ptr);

							if (found == 0) {
								/* Add the new state to the list */
								if (moves->state_ptr->id == -1) {
									/* Populate the head state */
									moves->state_ptr->id = state_iter->id;
								} else {
									/* Create a new state */
									new_state = malloc(sizeof(struct state));
									list_init(new_state);

									/* Populate the state id */
									new_state->id = state_iter->id;

									/* Add the new state to linked list */
									list_add(moves->state_ptr, new_state);
								}
							}
						} else {
							break;
						}

						/* Proceed to next state */
						state_iter = state_iter->next;
					} while (state_iter != node->state_ptr);

					break;
				}

				depth++;
			}

			/* Proceed to next state */
			state_iter = state_iter->next;
		} while (state_iter != input_states);

		/* Increment the id */
		id++;
	}

	return;
}

/* state_not_marked
 * Helper function for finding if a state has been marked
 */
int state_not_marked(state_t *state, struct list_head *state_list, int *state_id)
{
	int 		result = 1;
	int		index = 0;
	int 		id_found = 0;
	state_t 	*state_iter = state;
	state_t 	*iter_1, *iter_2;
	state_list_t 	*list_iter;

	/* Iterate over linked list of linked lists of states */
	list_for_each_entry(list_iter, state_list, list) {
		/* Keep track of state count */
		index++;

		/* Get the first linked list of states */
		state_iter = list_iter->state_ptr;

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
					*state_id = index;
					return 0;
				}

				/* Proceed to the next state */
				iter_1 = iter_1->next;
			} while (iter_1 != state_iter);
		} 
	}

	/* Update state id */
	*state_id = index + 1;

	return result;
}
