/**
 * SD, 2023
 *
 * Lab 09 - BST & Heap
 *
 * Binary Search Tree implementation
 */

// Copyright 2023 Nastase Cristian-Gabriel 315CA

#include "bst_lab09.h"

bst_node_t *__bst_node_create(void *data, size_t data_size)
{
	bst_node_t *bst_node;

	bst_node = malloc(sizeof(*bst_node));

	DIE(!bst_node, "bst_node malloc");

	bst_node->left = NULL;
	bst_node->right = NULL;

	bst_node->data = malloc(data_size);
	DIE(!bst_node->data, "bst_node->data malloc");
	memcpy(bst_node->data, data, data_size);

	return bst_node;
}

bst_tree_t *bst_tree_create(size_t data_size,
							int (*cmp_f)(const void *, const void *))
{
	bst_tree_t *bst_tree;

	bst_tree = malloc(sizeof(*bst_tree));
	DIE(!bst_tree, "bst_tree malloc");

	bst_tree->root  = NULL;
	bst_tree->data_size = data_size;
	bst_tree->cmp = cmp_f;

	return bst_tree;
}

void bst_tree_insert(bst_tree_t *bst_tree, void *data, int coordinates)
{
	int rc, contor = -1;
	bst_node_t *root = bst_tree->root, *parent = NULL;
	bst_node_t *node	= __bst_node_create(data, bst_tree->data_size);

	while (root) {
		parent = root;
		contor++;
		// mai departe compara in functie de numarul de coordonate
		if (coordinates == 2) {
			int c1, c2, root_c1, root_c2, test;
			test = sscanf(data, "%d %d", &c1, &c2);
			if (test != 2)
				return;
			test = sscanf(root->data, "%d %d", &root_c1, &root_c2);
			if (test != 2)
				return;

			// daca contorul este par, compara dupa prima coordonata
			// altfel, compara dupa a doua coordonata
			if (contor % 2 == 0)
				rc = bst_tree->cmp(&c1, &root_c1);
			else
				rc = bst_tree->cmp(&c2, &root_c2);
		} else if (coordinates == 3) {
			int c1, c2, c3, root_c1, root_c2, root_c3, test;
			test = sscanf(data, "%d %d %d", &c1, &c2, &c3);
			if (test != 3)
				return;
			test = sscanf(root->data, "%d %d %d", &root_c1, &root_c2, &root_c3);
			if (test != 3)
				return;
			// daca contorul este multiplu de 3, compara dupa prima coordonata
			// pt 3 + 1, compara dupa a doua coordonata, altfel, pt a treia
			if (contor % 3 == 0)
				rc = bst_tree->cmp(&c1, &root_c1);
			else if (contor % 3 == 1)
				rc = bst_tree->cmp(&c2, &root_c2);
			else
				rc = bst_tree->cmp(&c3, &root_c3);
		}
		if (rc < 0)
			root = root->left;
		else if (rc >= 0)
			root = root->right;
	}
	if (!parent) {
		bst_tree->root = node;
	} else {
		if (coordinates == 2) {
			int c1, c2, parent_c1, parent_c2, test;
			test = sscanf(data, "%d %d", &c1, &c2);
			test = sscanf(parent->data, "%d %d", &parent_c1, &parent_c2);
			if (test != 2)
				return;

			if (contor % 2 == 0)
				rc = bst_tree->cmp(&c1, &parent_c1);
			else
				rc = bst_tree->cmp(&c2, &parent_c2);
		} else if (coordinates == 3) {
			int c1, c2, c3, parent_c1, parent_c2, parent_c3, test;
			test = sscanf(data, "%d %d %d", &c1, &c2, &c3);
			test = sscanf(parent->data, "%d %d %d", &parent_c1,
						  &parent_c2, &parent_c3);
			if (test != 3)
				return;

			if (contor % 3 == 0)
				rc = bst_tree->cmp(&c1, &parent_c1);
			else if (contor % 3 == 1)
				rc = bst_tree->cmp(&c2, &parent_c2);
			else
				rc = bst_tree->cmp(&c3, &parent_c3);
		}
		if (rc < 0)
			parent->left = node;
		else
			parent->right = node;
	}
}

static void __bst_tree_free(bst_node_t *bst_node, void (*free_data)(void *))
{
	if (!bst_node)
		return;

	__bst_tree_free(bst_node->left, free_data);
	__bst_tree_free(bst_node->right, free_data);

	free_data(bst_node->data);
	free(bst_node);
}

void print_data(void *data)
{
	printf("(%s)\n", (char *)data);
}

void bst_tree_free(bst_tree_t *bst_tree, void (*free_data)(void *))
{
	__bst_tree_free(bst_tree->root, free_data);
	free(bst_tree);
}

int bst_cmp_num(const void *key1, const void *key2)
{
	int num1 = *(int *)key1;
	int num2 = *(int *)key2;

	if (num1 < num2)
		return -1;
	else if (num1 > num2)
		return 1;
	else
		return 0;
}
