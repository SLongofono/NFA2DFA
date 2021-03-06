#ifndef __COMMON_H__
#define __COMMON_H__

 /* Include library headers */
#include <stdio.h>
#include <stdlib.h>

/* Include local header */
#include "list.h"

/* Declare line-width which this program can handle */
#define LINE_WIDTH	(int)256

/* Convenience Macros for differntiating between different parsing rules */
#define SINGLE		(int)1
#define BRACES		(int)2

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
 * Convenience macro for initializing a state-list node
 */
#define state_list_init(head)					\
do {								\
	(head)->state_ptr = NULL;				\
	INIT_LIST_HEAD(&((head)->list));			\
} while (0);

/*
 * Convenience macro for initializing a dfa table entry for a
 * given state
 */
#define dfa_entry_init(dfa_entry, trans_id)			\
do {								\
	(dfa_entry)->id = trans_id;				\
	INIT_LIST_HEAD(&((dfa_entry)->dfa_transitions));	\
	INIT_LIST_HEAD(&((dfa_entry)->list));			\
} while (0);

/*
 * Convenience macro for initializing a dfa move
 */
#define dfa_move_init(dfa_move)					\
do {								\
	(dfa_move)->id = 0;					\
	INIT_LIST_HEAD(&((dfa_move)->list));			\
} while (0);

/*
 * DATA STRUCTURES
 *
 * Because of the generic nature of input text to this program,
 * the primary data structure used for storing information is
 * linked list based
 */
typedef struct state {
	int			id;
	int			size;
	struct state 		*next;
	struct state 		*prev;
} state_t;

typedef struct state_list {
	struct state 		*state_ptr;
	struct list_head	list;
} state_list_t;

typedef struct dfa_move_list {
	int			id;
	struct list_head	list;
} dfa_move_t;

typedef struct dfa_table_entry {
	int			id;
	struct list_head	dfa_transitions;
	struct list_head	list;
} dfa_entry_t;

#endif /* __COMMON_H__ */
