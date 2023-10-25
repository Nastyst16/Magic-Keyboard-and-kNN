// Copyright 2023 Nastase Cristian-Gabriel 315CA
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

typedef struct bst_node_t bst_node_t;
struct  bst_node_t {
	/* left child */
	bst_node_t *left;

	/* right child */
	bst_node_t *right;

	/* data contained by the node */
	void *data;
};

typedef struct bst_tree_t bst_tree_t;
struct bst_tree_t {
	/* root of the tree */
	bst_node_t  *root;

	 /* size of the data contained by the nodes */
	size_t data_size;

	/* function used for sorting the keys */
	int	(*cmp)(const void *key1, const void *key2);
};

// functions:
bst_node_t *__bst_node_create(void *data, size_t data_size);

bst_tree_t *bst_tree_create(size_t data_size,
							int (*cmp_f)(const void *, const void *));

void bst_tree_insert(bst_tree_t *bst_tree, void *data, int coordinates);

void bst_tree_free(bst_tree_t *bst_tree, void (*free_data)(void *));
int bst_cmp_num(const void *key1, const void *key2);
