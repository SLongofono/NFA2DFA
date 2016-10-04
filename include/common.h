#ifndef __COMMON_H__
#define __COMMON_H__

 /* Include library headers */
#include <stdio.h>
#include <stdlib.h>

/* Include local header */
#include "list.h"

typedef struct state {
	int			id;
	int			size;
	struct state 		*next;
	struct state 		*prev;
} state_t;

typedef struct transitions {
	int			id;
	struct state 		*a;
	struct state 		*b;
	struct state 		*E;
} transitions_t;

typedef struct state_list {
	struct state 		*state_ptr;
	struct list_head	list;
} state_list_t;

typedef struct dfa_table_entry {
	int			id;
	int			trans_a;
	int			trans_b;
	struct list_head	list;
} dfa_entry_t;

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

/*
 * Convenience macro for initializing a dfa table entry for a
 * given state
 */
#define dfa_entry_init(dfa_transition, trans_id)		\
do {								\
	(dfa_transition)->id = trans_id;			\
	(dfa_transition)->trans_a = 0;				\
	(dfa_transition)->trans_b = 0;				\
	INIT_LIST_HEAD(&((dfa_transition)->list));		\
} while (0);

/*
 * Convenience macro for initializing a state-list node
 */
#define state_list_init(head)					\
do {								\
	(head)->state_ptr = NULL;				\
	INIT_LIST_HEAD(&((head)->list));			\
} while (0);

#endif /* __COMMON_H__ */
