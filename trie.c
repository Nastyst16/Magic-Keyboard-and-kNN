// Copyright 2023 Nastase Cristian-Gabriel 315CA
/*
	SD 2023 - Trie
*/

#include "trie.h"

void free_value_cb(void *value)
{
	free(value);
	value = NULL;
}

trie_node_t *trie_create_node(void)
{
	trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
	DIE(!node, "Error allocating memory for trie node");

	node->value = 0;
	node->end_of_word = 0;
	node->n_children = 0;
	node->children = NULL;

	return node;
}

trie_t *trie_create(int data_size, int alphabet_size, char *alphabet)
{
	trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
	DIE(!trie, "Error allocating memory for trie");

	// alocarea memoriei pentru valoarea nodului radacina
	trie->root = trie_create_node();
	trie->root->value = malloc(sizeof(int));
	DIE(!trie->root->value, "Error allocating memory for trie root value");
	*(int *)(trie->root->value) = -1;

	// setarea campurilor din trie
	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;
	trie->alphabet = alphabet;
	trie->free_value_cb = free_value_cb;
	trie->n_nodes = 1;

	return trie;
}

// insert helper
void trie_insert_helper(trie_t *trie, trie_node_t *node, char *key,
						int data_size, int alphabet_size, char *alphabet)
{
	if (!node)
		return;

	char letter = *key;

	if (letter == '\0') {
		if (node->end_of_word == 0) {
			node->value = malloc(data_size);
			DIE(!node->value, "Error allocating memory for trie node value");
			(*(unsigned int *)node->value) = 0;
		}
		(*(unsigned int *)node->value)++;

		node->end_of_word = 1;

		return;
	}

	if (!node->children) {
		node->children = malloc(alphabet_size * sizeof(trie_node_t *));
		DIE(!node->children, "Error allocating memory for trie node children");
		for (int i = 0; i < alphabet_size; i++)
			node->children[i] = NULL;
	}

	if (!node->children[letter - 'a']) {
		node->children[letter - 'a'] = trie_create_node();
		++trie->n_nodes;
		++node->n_children;
	}

	trie_insert_helper(trie, node->children[letter - 'a'], key + 1,
					   data_size, alphabet_size, alphabet);
}

void trie_insert(trie_t *trie, char *key)
{
	if (!trie)
		return;

	trie_node_t *node = trie->root;
	trie->size++;

	trie_insert_helper(trie, node, key, trie->data_size,
					   trie->alphabet_size, trie->alphabet);
}

void *trie_search(trie_t *trie, char *key)
{
	trie_node_t *node = trie->root;
	char letter = *key;

	if (letter == '\0')
		return trie->root->value;

	while (node->children[letter - 'a']) {
		node = node->children[letter - 'a'];
		++key;
		letter = *key;

		if (letter == '\0')
			break;
	}

	if (node->end_of_word == 1)
		return node->value;
	else
		return NULL;
}

void trie_remove_helper(trie_t *trie, trie_node_t *node, char *key, int index)
{
	if (index == (int)strlen(key)) {
		if (node->end_of_word == 1) {
			node->end_of_word = 0;
			trie->size--;
			if (node->n_children == 0) {
				free(node->children);
				node->children = NULL;
				free(node->value);
				node->value = NULL;
			}
		}
		return;
	}
	if (node->children[key[index] - 'a'] == NULL)
		return;

	trie_remove_helper(trie, node->children[key[index] - 'a'], key, index + 1);
	if (node->children[key[index] - 'a']->n_children == 0 &&
		node->children[key[index] - 'a']->end_of_word == 0) {
		free(node->children[key[index] - 'a']->value);
		node->children[key[index] - 'a']->value = NULL;

		free(node->children[key[index] - 'a']->children);
		node->children[key[index] - 'a']->children = NULL;

		free(node->children[key[index] - 'a']);
		node->children[key[index] - 'a'] = NULL;
		node->n_children--;
		trie->n_nodes--;
	}
}

void trie_remove(trie_t *trie, char *key)
{
	trie_node_t *node = trie->root;
	trie_remove_helper(trie, node, key, 0);
}

void trie_free_helper(trie_node_t *node)
{
	if (node->children) {
		for (int i = 0; i < 26; i++)
			if (node->children[i])
				trie_free_helper(node->children[i]);
		free(node->children);
		node->children = NULL;
	}

	if (node->value)
		free_value_cb(node->value);

	free(node);
	node = NULL;
}

void trie_free(trie_t **p_trie)
{
	trie_t *trie = *p_trie;
	trie_node_t *node = trie->root;

	if (node->children) {
		for (int i = 0; i < trie->alphabet_size; i++)
			if (node->children[i])
				trie_free_helper(node->children[i]);
		free(node->children);
		node->children = NULL;
	}

	if (trie->root) {
		free_value_cb(trie->root->value);
		free(trie->root);
		trie->root = NULL;
	}

	free(trie);
	*p_trie = NULL;
}
