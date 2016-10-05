/*
 * Course		: EECS665
 * Name			: Waqar Ali
 * ID			: 2873101
 *
 * File
 * 	parser.c
 *
 * Description
 * 	Source file which contains implementation of a simple
 * 	parser for extracting data from input nfa-transitions
 */
 
/* Include local headers */
#include "parser.h"
 
/*
 * get_transitions
 * Helper function for parsing the transition table
 */
void get_transitions(char *input_buffer, struct list_head *states)
{
	int 		iter = 0;
	int 		flag = 1;
	int 		id = 0;
	int 		state_iter = 0;
	state_list_t 	*list_iter;
	state_t		*new_state;

	/* Extract the transition values from the input string */
	for (; iter < 256 && input_buffer[iter] != '\n'; iter++) {
		/* Find out if the state machine is in an accepting state */
		if (input_buffer[iter] == '{') {
			flag = 1;
			id = 0;
			state_iter = 0;

			/* Create a list node in linked list of states */
			list_iter = malloc(sizeof(struct state_list));
			state_list_init(list_iter);

			/* Create a new state node */
			new_state = malloc(sizeof(struct state));
			list_init(new_state);

			/* Track the state in the linked list of states */
			list_iter->state_ptr = new_state;

			/* Add the node of linked list of states to the output list */
			list_add_tail(&(list_iter->list), states);
		} else if (input_buffer[iter] == '}') {
			/* Store the last state */
			if (state_iter == 0) {
				continue;
			} else {
				/* Update the linked list */
				if (new_state->id == -1) {
					/* Populate the head node */
					new_state->id = id;
				} else {
					/* Create a new state */
					state_t *next_state = malloc(sizeof(struct state));

					/* Populate the id for this state */
					next_state->id = id;

					/* Place the state in the a-linked list */
					list_add(new_state, next_state);
				}
			}

			/* Set the flag to zero to stop looking for input states */
			flag = 0;
		}

		/* Proceed if we are in an accepting state */
		if (flag == 1) {
			/* Discard the brace character */
			if (input_buffer[iter] != '{') {
				if (input_buffer[iter] != ',') {
					/* Accumulate the string containing the string literals */
					id = (input_buffer[iter] - '0') + state_iter * 10 * id;
					state_iter++;
				} else {
					/* Update the linked list */
					if (new_state->id == -1) {
						new_state->id = id;
					} else {
						/* Create a new state */
						state_t *next_state = malloc(sizeof(struct state));

						/* Populate the id for this state */
						next_state->id = id;

						/* Place the new state in the linked list */
						list_add(new_state, next_state);
					}

					/* Reset the state iter */
					state_iter = 0;

					/* Reset the id value */
					id = 0;
				}
			}
		}
	}

	/* All done here */
	return;
}

/*
 * A convenience function for extracting the state values out of
 * a character buffer
 */
void get_states(char *input_buffer, state_t *in_head, int rule_type, int *total_states)
{
	int	temp_id = 0;
	int	state_iter = 0;
	int	flag = 0;
	int	iter = 0;

	/* Extract the input state from the line */
	for (iter = 0; iter < 256 && input_buffer[iter] != '\n'; iter++) {
		if (rule_type == BRACES) {
			/* Look for the braces to get the state value */
			if (input_buffer[iter] == '{') {
				flag = 1;
			} else if (input_buffer[iter] == '}') {
				/* Store the last state */
				if (state_iter == 0) {
					/* Fatal Error : No input state(s) */
					fprintf(stderr, "No valid input states found!\n");
					return;
				} else {
					/* Update the linked list */
					if (in_head->id == -1) {
						/* Populate the head state */
						in_head->id = temp_id;
					} else {
						/* Create a new state */
						state_t *next_state = malloc(sizeof(struct state));

						/* Populate the id for this state */
						next_state->id = temp_id;

						/* Place the new state in the linked list */
						list_add(in_head, next_state);
					}

					/* Reset the state iter */
					state_iter = 0;

					/* Reset the temp_id value */
					temp_id = 0;
				}

				/* Set the flag to zero to stop looking for input states */
				flag = 0;
			}

			if (flag == 1) {
				/* Discard the brace character */
				if (input_buffer[iter] !=  '{') {
					if (input_buffer[iter] != ',') {
						/* Accumulate the string containing the state literals */
						temp_id = (input_buffer[iter] - '0') + state_iter * 10 * temp_id;
						state_iter++;
					} else {
						/* Update the linked list */
						if (in_head->id == -1) {
							/* Populate the head state */
							in_head->id = temp_id;
						} else {
							/* Create a new state */
							state_t *next_state = malloc(sizeof(struct state));

							/* Populate the id for this state */
							next_state->id = temp_id;

							/* Place the new state in the linked list */
							list_add(in_head, next_state);
						}

						/* Reset the state iter */
						state_iter = 0;

						/* Reset the temp_id value */
						temp_id = 0;
					}
				}
			}
		} else if (rule_type == SINGLE) {
			/* Look for a single string of digits after ':' character */
			if (input_buffer[iter] == ':') {
				flag = 1;
			}

			if (flag == 1) {
				/* Discard any whitespaces */
				if (input_buffer[iter] < '0' || input_buffer[iter] > '9') {
					continue;
				} else {
					*total_states = (input_buffer[iter] - '0') + state_iter * 10 * (*total_states);
					state_iter++;
				}
			}
		}

	}

	/* All done here */
	return;
}
