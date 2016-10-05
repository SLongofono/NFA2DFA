/*
 * Course		: EECS665
 * Name			: Waqar Ali
 * ID			: 2873101
 *
 * File
 * 	main.c
 *
 * Description
 * 	NFA2DFA Conversion Algorithm (Assignment # 1)
 */

/* Include local headers */
#include "parser.h"
#include "nfa2dfa.h"
#include "helper_functions.h"

/* Declare debug controls for this file */
#define MAIN_DEBUG	0

/*
 * Main routine of this program
 */
int main(void)
{
	char	input_buffer[256];
	int	total_states = 0;
	int	iter = 0;

#if (MAIN_DEBUG == 1)
	/* Declare debug data */
	int	debug_header = 0;
	char	symbol = 'a';
#endif

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
	struct list_head *state_transitions = malloc(total_states * sizeof(struct list_head));

	for (iter = 0; iter < total_states; iter++) {
		/* Get the first state transition */
		fgets(input_buffer, 256, stdin);

		/* Initialize the linked list heads */
		INIT_LIST_HEAD(&(state_transitions[iter]));

		/* Parse the state transition string to get the out-going transitions */
		get_transitions(input_buffer, &(state_transitions[iter]));

#if (MAIN_DEBUG == 1)
		/* DEBUG */
		state_list_t *temp;
		state_list_t *tail = list_entry((&(state_transitions[iter]))->prev, state_list_t, list);

		if (debug_header == 0) {
			printf("********************* DEBUG BEGIN *************** \n");
			printf("The Program sees the input NFA table like this:\n\n");

			/* Print the symbol header */
			debug_header = 1;
			printf("State	");

			list_for_each_entry(temp, &(state_transitions[iter]), list) {
				if (temp == tail) {
					printf("E");
				} else {
					printf("%c	", symbol);
				}

				symbol++;
			}

			/* Print a new line */
			printf("\n");
		}

		printf("%d	", iter+1);
		list_for_each_entry(temp, &(state_transitions[iter]), list) {
			printf("{");
			print_state(temp->state_ptr);
			printf("}	");
		}

		printf("\n");
#endif /* MAIN_DEBUG */
	}

#if (MAIN_DEBUG == 1)
			/* Print the bottom debug header */
			printf("********************** DEBUG END **************** \n\n");
#endif

	/* Create a linked list of unmarked states */
	LIST_HEAD(unmarked_states);

	/* Create the first node for unmarked states list */
	state_list_t *um_state = malloc(sizeof(struct state_list));
	state_list_init(um_state);

	/* Calculate e-closure of Initial State */
	state_t *list;
	e_closure(in_head, state_transitions, 1, &list);
	printf ("E-Closure(I0) = {");
	print_state(list);
	printf ("} = %d\n", 1);

	/* Populate the unmarked state node */
	um_state->state_ptr = list;

	/* Push the obtained linked list to the unmarked states */
	list_add_tail(&(um_state->list), &(unmarked_states));

	/* DFA states iterator */
	int dfa_state = 0;
	int total_dfa_states = 1;

	/* Track DFA transitions */
	LIST_HEAD(dfa);

	/* Iterate over list of unmarked states */
	list_for_each_entry(um_state, &unmarked_states, list) {
		/* Crate a new dfa table node */
		dfa_entry_t *dfa_node;

		/* Allocate memory for dfa node */
		dfa_node = malloc(sizeof(struct dfa_table_entry));

		/* Initialize list node */
		dfa_entry_init(dfa_node, dfa_state+1);

		/* Add the dfa node to dfa table */
		list_add_tail(&(dfa_node->list), &dfa);

		/* Create a linked list of moves */
		LIST_HEAD(moves);

		/* Mark the current state */
		printf("\nMark-%d\n", dfa_state+1);
		mark(um_state->state_ptr, state_transitions, &moves);

		char depth = 'a';
		state_list_t *move_list;
	
		/* Iterate over input symbols and calculate dfa states */
		list_for_each_entry(move_list, &moves, list) {
			/* Create a node in the dfa state linked list */
			dfa_move_t *dfa_move;

			/* Allocate memory for the dfa move */
			dfa_move = malloc(sizeof(struct dfa_move_list));

			/* Initialize the dfa move */
			dfa_move_init(dfa_move);

			/* Add the move to the dfa table entry */
			list_add_tail(&(dfa_move->list), &(dfa_node->dfa_transitions));

			/* Make sure that the linked list contain legitimate data */
			if (move_list->state_ptr->id != -1) {
				printf("{");
				print_state(um_state->state_ptr);
				printf("} --%c--> {", depth);
				print_state(move_list->state_ptr);
				printf("}\n");

				/* Calculate e-closure of this state */
				e_closure(move_list->state_ptr, state_transitions, 1, &list);

				printf("E-closure{");
				print_state(move_list->state_ptr);
				printf("} = {");
				print_state(list);
				printf("}");

				/* Find out if the new state is already present in the state-list */
				if (state_not_marked(list, &unmarked_states)) {
					/* Keep track of total dfa-states */
					total_dfa_states++;

					/* Create a node in the unmarked states list */
					state_list_t *new_state_list = malloc(sizeof(struct state_list));
					state_list_init(new_state_list);

					/* Populate the node with the linked list value */
					new_state_list->state_ptr = list;

					/* Put the new list node in the unmarked states list */
					list_add_tail(&(new_state_list->list), &(unmarked_states));
				}

				/* Keep track of dfa table transitions */
				dfa_move->id = total_dfa_states;

				/* Print total dfa states counted so far */
				printf(" = %d\n", total_dfa_states);
			}

			depth++;
		}

		/* Keep track of dfa state being processed */
		dfa_state++;
	}

	printf("\n");

	/* Now print out the DFA Transition Table */
	print_dfa_table(&dfa);

	/* All done here */
	return 0;
}
