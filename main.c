/*
 * Course		: EECS665
 * Name			: Waqar Ali
 * ID			: 2873101
 *
 * Description
 * 	NFA2DFA Conversion Algorithm (Assignment # 1)
 */

#include <stdio.h>
#include <stdlib.h>

/* Declare the master debug control */
#define DEBUG	0

typedef struct state {
	int	id;
	int	size;
	struct state *next;
	struct state *prev;
} state_t;

typedef struct transitions {
	int	id;
	struct state *a;
	struct state *b;
	struct state *E;
} transitions_t;

typedef struct state_list {
	int size;
	struct state *head;
	struct state_list *next;
	struct state_list *prev;
} state_list_t;


/* Convenience Macros for differntiating between different parsing rules */
#define SINGLE		(int)1
#define BRACES		(int)2

/*
 * Convenience macro for intializing the head of a circular doubly
 * linked list
 */
#define list_init(head)						\
do {								\
	(head)->id = -1;					\
	(head)->size = 1;					\
	(head)->next = (head);					\
	(head)->prev = (head);					\
} while (0);

#define state_list_init(list_head)				\
do {								\
	(list_head)->head = NULL;				\
	(list_head)->size = 1;					\
	(list_head)->next = (list_head);			\
	(list_head)->prev = (list_head);			\
} while (0);

/*
 * Convenience macro for adding a node to a circular doubly linked
 * list
 */
#define list_add(list_head, node)				\
do {								\
	list_head->size++;					\
	if (list_head->prev == list_head) {			\
		node->prev = list_head;				\
		node->next = list_head;				\
		list_head->next = node;				\
		list_head->prev = node;				\
	} else {						\
		list_head->prev->next = node;			\
		node->prev = list_head->prev;			\
		list_head->prev = node;				\
		node->next = list_head;				\
	}							\
} while (0);

/* Utility Functions */
void get_states(char *input_buffer, state_t *in_head, int rule_type, int *total_states);
void get_transitions(char *input_buffer, transitions_t *transitions);
int state_not_marked(state_t *state, state_list_t *list_head);

/* Primary Functions for NFA2DFA Conversion */
void e_closure(state_t *input_states, transitions_t **transitions, int empty, state_t **output_states);
void mark(state_t *input_states, transitions_t **transitions, state_t **a_states, state_t **b_states);

/* Debug Functions */
void debug_print_states(state_t *head);

/*
 * Main routine of this program
 */
int main(void)
{
	char	input_buffer[256];
	int	total_states = 0;
	int	iter = 0;

	/* Create head-state for start states */
	state_t *in_head;
	in_head = malloc(sizeof(struct state));

	/* Crate head-state for finish states */
	state_t *out_head;
	out_head = malloc(sizeof(struct state));

	/* Initialize the head of linked list of start states */
	list_init(in_head);

	/* Initialize the head of linked list of finish states */
	list_init(out_head);

	/* Get the input state from the stdin */
	fgets(input_buffer, 256, stdin);

	/* Parse the input string to get the input states */
	get_states(input_buffer, in_head, BRACES, NULL);

	/* Get the finish states from the stdin */
	fgets(input_buffer, 256, stdin);

	/* Parse the input string to get the input final states */
	get_states(input_buffer, out_head, BRACES, NULL);

	/* Get the total states from the stdin */
	fgets(input_buffer, 256, stdin);

	/* Parse the next input string to get the total number of states */
	get_states(input_buffer, NULL, SINGLE, &total_states);

	/* Consume the next line - Taking input symbols 'a' and 'b' for granted at the moment */
	fgets(input_buffer, 256, stdin);

	/* Create a dynamic array for storing the transitions for all NFA states*/
	transitions_t **transitions = malloc(total_states * sizeof(struct transitions *));

	for (iter = 0; iter < total_states; iter++) {
		/* Get the first state transition */
		fgets(input_buffer, 256, stdin);

		/* Declare a transition variable */
		transitions[iter] = malloc(sizeof(struct transitions));
		transitions[iter]->a = malloc(sizeof(struct state));
		transitions[iter]->b = malloc(sizeof(struct state));
		transitions[iter]->E = malloc(sizeof(struct state));
		list_init(transitions[iter]->a);
		list_init(transitions[iter]->b);
		list_init(transitions[iter]->E);

		/* Parse the state transition string to get the out-going transitions */
		get_transitions(input_buffer, transitions[iter]);
	}

	/* Create a transition table for dfa states */
	transitions_t **dfa_transitions = malloc(sizeof(struct transitions *));

	/* Create a linked list of unmarked states */
	state_list_t *unmarked_states = malloc(sizeof(struct state_list));
	state_list_init(unmarked_states);

	/* Calculate e-closure of Initial State */
	state_t *list;
	e_closure(in_head, transitions, 1, &list);

	/* Push the obtained linked list to the unmarked states */
	unmarked_states->head = list;

	/* Iterate over unmarked states */
	state_list_t *list_iter = unmarked_states;

	do {
		printf("\n*** Marking State:\n");
		debug_print_states(list_iter->head);

		state_t *a_move, *b_move;

		/* Mark the current state */
		mark(list_iter->head, transitions, &a_move, &b_move);

		if (a_move->id != -1) {
			/* Calculate e-closure of a-move */
			e_closure(a_move, transitions, 1, &list);

			/* Find out if the new state is already present in the state-list */
			if (state_not_marked(list, unmarked_states)) {
				/* Create a node in the unmarked states list */
				state_list_t *new_state_list = malloc(sizeof(struct state_list));

				/* Populate the node with the linked list value */
				new_state_list->head = list;

				/* Put the new list node in the unmarked states list */
				list_add(unmarked_states, new_state_list);
			}
		}
	
		if (b_move->id != -1) {
			/* Calculate e-closure of b-move */
			e_closure(b_move, transitions, 1, &list);

			/* Find out if the new state is already present in the state-list */
			if (state_not_marked(list, unmarked_states)) {
				/* Create a node in the unmarked states list */
				state_list_t *new_state_list = malloc(sizeof(struct state_list));

				/* Populate the node with the linked list value */
				new_state_list->head = list;

				/* Put the new list node in the unmarked states list */
				list_add(unmarked_states, new_state_list);
			}
		}

		/* Proceed to the next state in the list */
		list_iter = list_iter->next;
	} while (list_iter != unmarked_states);

	printf("\n");

#if DEBUG
	/* Calculate E-closure of the initial state */
	state_t *list;
	e_closure(in_head, transitions, 1, &list);

	printf("E-Closure of I0:\n");
	debug_print_states(list);

	/* Mark the calculated state */
	state_t *a_move, *b_move;
	mark(list, transitions, &a_move, &b_move);

	printf("Move-A of 1:\n");
	debug_print_states(a_move);

	printf("Move-B of 1:\n");
	debug_print_states(b_move);

	e_closure(a_move, transitions, 1, &list);
	printf("E-Closure of 1:\n");
	debug_print_states(list);

	mark(list, transitions, &a_move, &b_move);

	printf("Move-A of 2:\n");
	debug_print_states(a_move);

	printf("Move-B of 2:\n");
	debug_print_states(b_move);

	e_closure(b_move, transitions, 1, &list);
	printf("E-Closure of 4:\n");
	debug_print_states(list);

	/* DEBUG */
	printf("\n*** Initial States \n");
	debug_print_states(in_head);

	printf("\n*** Final States   \n");
	debug_print_states(out_head);

	printf("\n*** Total States   = %d\n", total_states);

	for (iter = 0; iter < total_states; iter++) {
		printf("\n*** Transition     = %d\n", transitions[iter]->id);
		printf("\n*** Transition->a  \n");
		debug_print_states(transitions[iter]->a);
		printf("\n*** Transition->b  \n");
		debug_print_states(transitions[iter]->b);
		printf("\n*** Transition->E  \n");
		debug_print_states(transitions[iter]->E);
		printf("\n");
	}
#endif

	/* All done here */
	return 0;
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
 * get_transitions
 * Helper function for parsing the transition table
 */
void get_transitions(char *input_buffer, transitions_t *transitions)
{
	int iter = 0;
	int flag = 1;
	int id_flag = 0;
	int a_flag = 0;
	int b_flag = 0;
	int E_flag = 0;
	int id = 0;
	int state_iter = 0;

	/* Extract the transition values from the input string */
	for (iter = 0; iter < 256 && input_buffer[iter] != '\n'; iter++) {
		/* Find out if the state machine is in an accepting state */
		if (id_flag == 0) {
			/* Extract the number from the current input string */
			if (input_buffer[iter] < '0' || input_buffer[iter] > '9') {
					/* No longer in the accepting state */
					id_flag = 1;

					/* Make sure that we have a legal value of id */
					if (id == 0) {
						/* id is invalid. Halt the program */
						fprintf(stderr, "ID could not be extracted from the transition table!\n");
						while(1);
					} else {
						/* Set the id for this transition */
						transitions->id = id;
					}
			} else {
				id = (input_buffer[iter] - '0') + state_iter * 10 * (id);
				state_iter++;
			}
		} else if (id_flag == 1 && a_flag == 0) {
			/* Id has been acquired. Get the a-transitions */
			if (input_buffer[iter] == '{') {
				flag = 1;
				id = 0;
				state_iter = 0;
			} else if (input_buffer[iter] == '}') {
				/* Mark the acquisition of a-transitions as complete */
				a_flag = 1;

				/* Store the last state */
				if (state_iter == 0) {
					continue;
				} else {
					/* Update the linked list */
					if (transitions->a->id == -1) {
						/* Populate the head node */
						transitions->a->id = id;
					} else {
						/* Create a new state */
						state_t *next_state = malloc(sizeof(struct state));

						/* Populate the id for this state */
						next_state->id = id;

						/* Place the state in the a-linked list */
						list_add(transitions->a, next_state);
					}
				}

				/* Set the flag to zero to stop looking for input states */
				flag = 0;
			}

			if (flag == 1) {
				/* Discard the brace character */
				if (input_buffer[iter] != '{') {
					if (input_buffer[iter] != ',') {
						/* Accumulate the string containing the string literals */
						id = (input_buffer[iter] - '0') + state_iter * 10 * id;
						state_iter++;
					} else {
						/* Update the linked list */
						if (transitions->a->id == -1) {
							transitions->a->id = id;
						} else {
							/* Create a new state */
							state_t *next_state = malloc(sizeof(struct state));

							/* Populate the id for this state */
							next_state->id = id;

							/* Place the new state in the linked list */
							list_add(transitions->a, next_state);
						}

						/* Reset the state iter */
						state_iter = 0;

						/* Reset the id value */
						id = 0;
					}
				}
			}
		} else if (id_flag == 1 && a_flag == 1 && b_flag == 0) {
			/* Id has been acquired. Get the a-transitions */
			if (input_buffer[iter] == '{') {
				flag = 1;
				id = 0;
				state_iter = 0;
			} else if (input_buffer[iter] == '}') {
				/* Mark the acquisition of a-transitions as complete */
				b_flag = 1;

				/* Store the last state */
				if (state_iter == 0) {
					continue;
				} else {
					/* Update the linked list */
					if (transitions->b->id == -1) {
						/* Populate the head node */
						transitions->b->id = id;
					} else {
						/* Create a new state */
						state_t *next_state = malloc(sizeof(struct state));

						/* Populate the id for this state */
						next_state->id = id;

						/* Place the state in the a-linked list */
						list_add(transitions->b, next_state);
					}
				}

				/* Set the flag to zero to stop looking for input states */
				flag = 0;
			}

			if (flag == 1) {
				/* Discard the brace character */
				if (input_buffer[iter] != '{') {
					if (input_buffer[iter] != ',') {
						/* Accumulate the string containing the string literals */
						id = (input_buffer[iter] - '0') + state_iter * 10 * id;
						state_iter++;
					} else {
						/* Update the linked list */
						if (transitions->b->id == -1) {
							transitions->b->id = id;
						} else {
							/* Create a new state */
							state_t *next_state = malloc(sizeof(struct state));

							/* Populate the id for this state */
							next_state->id = id;

							/* Place the new state in the linked list */
							list_add(transitions->b, next_state);
						}

						/* Reset the state iter */
						state_iter = 0;

						/* Reset the id value */
						id = 0;
					}
				}
			}
		} else if (id_flag == 1 && a_flag == 1 && b_flag == 1 && E_flag == 0) {
			/* Id has been acquired. Get the a-transitions */
			if (input_buffer[iter] == '{') {
				flag = 1;
				id = 0;
				state_iter = 0;
			} else if (input_buffer[iter] == '}') {
				/* Mark the acquisition of a-transitions as complete */
				E_flag = 1;

				/* Store the last state */
				if (state_iter == 0) {
					continue;
				} else {
					/* Update the linked list */
					if (transitions->E->id == -1) {
						/* Populate the head node */
						transitions->E->id = id;
					} else {
						/* Create a new state */
						state_t *next_state = malloc(sizeof(struct state));

						/* Populate the id for this state */
						next_state->id = id;

						/* Place the state in the E-linked list */
						list_add(transitions->E, next_state);
					}
				}

				/* Set the flag to zero to stop looking for input states */
				flag = 0;
			}

			if (flag == 1) {
				/* Discard the brace character */
				if (input_buffer[iter] != '{') {
					if (input_buffer[iter] != ',') {
						/* Accumulate the string containing the string literals */
						id = (input_buffer[iter] - '0') + state_iter * 10 * id;
						state_iter++;
					} else {
						/* Update the linked list */
						if (transitions->E->id == -1) {
							transitions->E->id = id;
						} else {
							/* Create a new state */
							state_t *next_state = malloc(sizeof(struct state));

							/* Populate the id for this state */
							next_state->id = id;

							/* Place the new state in the linked list */
							list_add(transitions->E, next_state);
						}

						/* Reset the state iter */
						state_iter = 0;

						/* Reset the id value */
						id = 0;
					}
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

/*
 * debug_print_states
 * Debug function for printing the state values
 */
void debug_print_states(state_t *head)
{
	int	state_number = 0;
	state_t	*node = head;
	do {
		/* If head contains a valid state, print it */
		if (node->id != -1) {
			printf("State [%.2d] : %.2d\n", state_number, node->id);
		}

		/* Advance to the next node in the list */
		node = node->next;
		state_number++;
	} while (node != head);

	/* All done here */
	return;
}
