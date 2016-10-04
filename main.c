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

	/* Create a linked list of unmarked states */
	state_list_t *unmarked_states = malloc(sizeof(struct state_list));
	state_list_init(unmarked_states);

	/* Calculate e-closure of Initial State */
	state_t *list;
	e_closure(in_head, transitions, 1, &list);
	printf ("E-Closure(I0) = {");
	print_state(list);
	printf ("} = %d\n", 1);

	/* Push the obtained linked list to the unmarked states */
	unmarked_states->head = list;

	/* Iterate over unmarked states */
	state_list_t *list_iter = unmarked_states;

	/* DFA states iterator */
	int dfa_state = 0;
	int total_dfa_states = 1;

	/* Track DFA transitions */
	dfa_entry_t *dfa_head = malloc(sizeof(struct dfa_table_entry));

	/* Initialize the dfa table entry */
	dfa_entry_init(dfa_head, (dfa_state+1));

	do {
		state_t *a_move, *b_move;
		dfa_entry_t *dfa_transition;

		/* Add the entry to dfa table */
		if (dfa_state != 0) {
			/* Allocate an entry for this state in the dfa-table */
			dfa_transition = malloc(sizeof(struct dfa_table_entry));

			/* Initialize the dfa table entry */
			dfa_entry_init(dfa_transition, (dfa_state+1));

			/* Add the node to dfa table */
			list_add(dfa_head, dfa_transition);
		} else {
			dfa_transition = dfa_head;
		}

		/* Mark the current state */
		printf("\nMark-%d\n", dfa_state+1);
		mark(list_iter->head, transitions, &a_move, &b_move);

		if (a_move->id != -1) {
			printf("{");
			print_state(list_iter->head);
			printf("} --a--> {");
			print_state(a_move);
			printf("}\n");

			/* Calculate e-closure of a-move */
			e_closure(a_move, transitions, 1, &list);
			printf("E-closure{");
			print_state(a_move);
			printf("} = {");
			print_state(list);
			printf("}");

			/* Find out if the new state is already present in the state-list */
			if (state_not_marked(list, unmarked_states)) {
				/* Keep track of total dfa-states */
				total_dfa_states++;

				/* Create a node in the unmarked states list */
				state_list_t *new_state_list = malloc(sizeof(struct state_list));

				/* Populate the node with the linked list value */
				new_state_list->head = list;

				/* Put the new list node in the unmarked states list */
				list_add(unmarked_states, new_state_list);
			}

			/* Keep track of dfa table transitions */
			dfa_transition->trans_a = total_dfa_states;

			/* Print total dfa states counted so far */
			printf(" = %d\n", total_dfa_states);
		}
	
		if (b_move->id != -1) {
			printf("{");
			print_state(list_iter->head);
			printf("} --b--> {");
			print_state(b_move);
			printf("}\n");

			/* Calculate e-closure of b-move */
			e_closure(b_move, transitions, 1, &list);
			printf("E-closure{");
			print_state(b_move);
			printf("} = {");
			print_state(list);
			printf("}");

			/* Find out if the new state is already present in the state-list */
			if (state_not_marked(list, unmarked_states)) {
				/* Keep track of total dfa-states */
				total_dfa_states++;

				/* Create a node in the unmarked states list */
				state_list_t *new_state_list = malloc(sizeof(struct state_list));

				/* Populate the node with the linked list value */
				new_state_list->head = list;

				/* Put the new list node in the unmarked states list */
				list_add(unmarked_states, new_state_list);
			}

			/* Keep track of dfa table transitions */
			dfa_transition->trans_b = total_dfa_states;

			/* Print total dfa states counted so far */
			printf(" = %d\n", total_dfa_states);
		}

		/* Proceed to the next state in the list */
		list_iter = list_iter->next;
		dfa_state++;
	} while (list_iter != unmarked_states);

	printf("\n");

	/* Now print out the DFA Transition Table */
	print_dfa_table(dfa_head);

	/* All done here */
	return 0;
}