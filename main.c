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

typedef struct state {
	int	id;
	struct state *next;
	struct state *prev;
} state_t;

/*
 * Convenience macro for intializing the head of a circular doubly
 * linked list
 */
#define list_init(head)						\
do {								\
	head->id = -1;						\
	head->next = head;					\
	head->prev = head;					\
} while (0);

/*
 * Convenience macro for adding a node to a circular doubly linked
 * list
 */
#define list_add(head, node)					\
do {								\
	if (head->prev == head) {				\
		node->prev = head;				\
		node->next = head;				\
		head->next = node;				\
		head->prev = node;				\
	} else {						\
		head->prev->next = node;			\
		node->prev = head->prev;			\
		head->prev = node;				\
		node->next = head;				\
	}							\
} while (0);

/* Utility Functions */
void get_states(char *input_buffer, state_t *in_head);

/*
 * Main routine of this program
 */
int main(void)
{
	char	input_buffer[256];
	int	iter;

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
	get_states(input_buffer, in_head);

	/* Get the finish states from the stdin */
	fgets(input_buffer, 256, stdin);

	/* Parse the input string to get the input final states */
	get_states(input_buffer, out_head);

	/* DEBUG */
	state_t *var_state = in_head;
	printf("Input State[%d] : %d\n", 0, var_state->id);

	iter = 1;
	while (var_state->next != in_head) {
		var_state = var_state->next;
		printf("Input State[%d] : %d\n", iter, var_state->id);
		iter++;
	}

	var_state = out_head;
	printf("Output State[%d]  : %d\n", 0, var_state->id);

	iter = 1;
	while (var_state->next != out_head) {
		var_state = var_state->next;
		printf("Output State[%d]  : %d\n", iter, var_state->id);
		iter++;
	}

	/* All done here */
	return 0;
}

/* 
 * A convenience function for extracting the state values out of
 * a character buffer
 */
void get_states(char *input_buffer, state_t *in_head)
{
	int	temp_id = 0;
	int	state_iter = 0;
	int	flag = 0;
	int	iter = 0;

	/* Extract the input state from the line */
	for (iter = 0; iter < 256 && input_buffer[iter] != '\n'; iter++) {
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
	}

	/* All done here */
	return;
}
