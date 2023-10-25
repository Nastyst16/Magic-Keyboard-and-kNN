// Copyright 2023 Nastase Cristian-Gabriel 315CA
#include "bst_lab09.h"
#include <math.h>
#define MAX_STRING_SIZE 64

typedef struct NN NN;
struct NN {
	char nearest_neigh[MAX_STRING_SIZE];
	float sum;
};

typedef struct RS RS;
struct RS {
	int p1_start, p1_end;
	int p2_start, p2_end;
	int p3_start, p3_end;
};

// functia load: in prima linie se afla un numar, care reprezinta numarul
// de perechi de coordonate. al doilea numar de pe prima linie este numarul
// de coordonate care trebuie cautate
void load_file(bst_tree_t *bst, int *pairs, int *coordinates)
{
	char input[MAX_STRING_SIZE];
	scanf("%s", input);
	FILE *in = fopen(input, "r");
	DIE(!in, "Couldn't open file :(\n");

	fscanf(in, "%d %d", pairs, coordinates);
	fgets(input, MAX_STRING_SIZE, in); // citeste \n

	for (int i = 0; i < *pairs; i++) {
		fgets(input, MAX_STRING_SIZE, in);
		input[strlen(input) - 1] = '\0';

		bst_tree_insert(bst, input, *coordinates);
	}

	fclose(in);
}

// nearest neighbour
void nn(bst_node_t *node, int p1, int p2, int p3,
		int coordinates, NN *neighbour)
{
	if (!node || !neighbour)
		return;

	// formula din cerinta
	// notam cu q coordonatele nodului fata de care calculam distanta
	// notam cu p coordonatele de la comanda
	// -> d(p, q);
	float sum = 0;

	int q1, q2, q3, test; // coordonatele din nod
	test = sscanf(node->data, "%d %d", &q1, &q2);
	if (test != 2)
		return;

	if (coordinates == 2) {
		// daca avem 2 coordonate, ignoram al 3-lea numar din nod
		sum = (q1 - p1) * (q1 - p1) + (q2 - p2) * (q2 - p2);

	} else if (coordinates == 3) {
		// ignoram primele 2 numere din nod si citim doar al 3-lea
		test = sscanf(node->data, "%*d %*d %d", &q3);

		// daca avem 3 coordonate, calculam distanta euclidiana
		sum = (q1 - p1) * (q1 - p1) + (q2 - p2) * (q2 - p2) +
			  (q3 - p3) * (q3 - p3);
	}
	sum = sqrt(sum);
	// daca gasim un nod cu distanta mai mica decat cea curenta
	// actualizam distanta si nodul. daca gasim un nod cu distanta egala
	// cu cea curenta, adaugam nodul cu strcat la stringul din structura
	if (sum < neighbour->sum) {
		neighbour->sum = sum;
		strcpy(neighbour->nearest_neigh, node->data);

	} else if (sum == neighbour->sum) {
		strcat(neighbour->nearest_neigh, "\n");
		strcat(neighbour->nearest_neigh, node->data);
	}

	// daca nodul curent este frunza, nu mai avem ce sa cautam
	if (!node->left && !node->right)
		return;

	nn(node->left, p1, p2, p3, coordinates, neighbour);
	nn(node->right, p1, p2, p3, coordinates, neighbour);
}

void rs(bst_node_t *node, int coordinates, RS *range_search)
{
	if (!node)
		return;

	rs(node->left, coordinates, range_search);
	rs(node->right, coordinates, range_search);

	if (coordinates == 2) {
		int q1, q2, test; // coordonatele din nod
		test = sscanf(node->data, "%d %d", &q1, &q2);
		if (test != 2)
			return;

		// daca nodul se afla in sectiunea ceruta, il afisam
		if (q1 >= range_search->p1_start && q1 <= range_search->p1_end &&
			q2 >= range_search->p2_start && q2 <= range_search->p2_end) {
			printf("%s\n", (char *)node->data);
			return;
		}

	} else if (coordinates == 3) {
		int q1, q2, q3, test; // coordonatele din nod
		test = sscanf(node->data, "%d %d %d", &q1, &q2, &q3);
		if (test != 3)
			return;

		// daca nodul se afla in sectiunea ceruta, il afisam
		if (q1 >= range_search->p1_start && q1 <= range_search->p1_end &&
			q2 >= range_search->p2_start && q2 <= range_search->p2_end &&
			q3 >= range_search->p3_start && q3 <= range_search->p3_end) {
			printf("%s\n", (char *)node->data);
			return;
		}
	}
}

int main(void)
{
	bst_tree_t *bst = bst_tree_create(MAX_STRING_SIZE, bst_cmp_num);

	int coordinates = 0, pairs = 0;
	char *command = malloc(MAX_STRING_SIZE);
	DIE(!command, "malloc failed\n");
	scanf("%s", command);

	if (strcmp(command, "LOAD") == 0)
		load_file(bst, &pairs, &coordinates);

	NN *neighbour = malloc(sizeof(NN));
	DIE(!neighbour, "malloc failed\n");

	RS *range_search = malloc(sizeof(RS));
	DIE(!range_search, "malloc failed\n");

	while (1) {
		scanf("%s", command);

		if (strcmp(command, "NN") == 0) {
			int p1, p2, p3;
			strcpy(neighbour->nearest_neigh, "");
			neighbour->sum = 99999;

			scanf("%d%d", &p1, &p2);
			if (coordinates == 3)
				scanf("%d", &p3);

			nn(bst->root, p1, p2, p3, coordinates, neighbour);

			printf("%s\n", (char *)neighbour->nearest_neigh);

		} else if (strcmp(command, "RS") == 0) {
			scanf("%d%d%d%d", &range_search->p1_start, &range_search->p1_end,
				  &range_search->p2_start, &range_search->p2_end);

			if (coordinates == 2) {
				rs(bst->root, coordinates, range_search);

			} else if (coordinates == 3) {
				scanf("%d%d", &range_search->p3_start, &range_search->p3_end);

				rs(bst->root, coordinates, range_search);
			}
		} else if (strcmp(command, "EXIT") == 0) {
			break;
		}
	}

	bst_tree_free(bst, free);
	free(command);
	free(neighbour);
	free(range_search);

	return 0;
}
