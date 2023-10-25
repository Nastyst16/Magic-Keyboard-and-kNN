// Copyright 2023 Nastase Cristian-Gabriel 315CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"

#define MAX_STRING_SIZE 64
#define DEBUG

void traverse_trie(trie_t *trie, trie_node_t *node, char *search, char *word,
				   int level, int k, int *ok, int command_type,
				   char *shortest_word, char *first_in_dict,
				   char *highest_freq_word, int *h_f_w_value, int word_freq)
{
	if (!node)
		return;

	// Verificați dacă nodul curent reprezintă un cuvânt complet
	if (node->end_of_word) {
		word_freq++;
		search[level] = '\0'; // Adăugați terminatorul de șir

		// am gasit un cuvant in trie
		int contor = 0;

		// 0 pt autoCORRECT
		if (command_type == 0 && strlen(word) == strlen(search)) {
			// verificam cu cate cifre difera cuvantul gasit, fata de
			// cuvantul caruia ii facem autocorrect
			for (int i = 0; i < (int)strlen(word); i++)
				if (search[i] != word[i])
					++contor;
			// daca diferenta e mai mica decat k, afisam cuvantul
			if (contor <= k) {
				printf("%s\n", search);
				*ok = 1;
				return;
			}
		// 1 pt autoCOMPLETE
		} else if (command_type == 1 && strlen(search) >= strlen(word)) {
			// verificam daca cuvantul gasit in trie incepe cu cuvantul
			// caruia ii facem autocomplete
			for (int i = 0; i < (int)strlen(word); i++)
				if (search[i] == word[i])
					++contor;
			// mai departe vom stoca in parametrii: shortest_word,
			// first_in_dict, highest_freq_word, cuvintele cerute in enunt
			if (contor == (int)strlen(word)) {
				if (strlen(search) < strlen(shortest_word))
					strcpy(shortest_word, search);
				if (*ok == 0)
					strcpy(first_in_dict, search);
				else if (strcmp(search, first_in_dict) < 0)
					strcpy(first_in_dict, search);

				if (*(int *)node->value > *h_f_w_value) {
					*h_f_w_value = *(int *)node->value;
					strcpy(highest_freq_word, search);
				}
				*ok = 1;
			}
		}
	}
	if (!node->children || word_freq == trie->size)
		return;

	// Parcurgeți toți copiii nodului curent
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (node->children[i]) {
			search[level] = 'a' + i; // Adăugați litera corespunzătoare
			traverse_trie(trie, node->children[i], search, word, level + 1, k,
						  ok, command_type, shortest_word, first_in_dict,
						  highest_freq_word, h_f_w_value, word_freq);
		}
}

void correct_or_complete(trie_t *trie, char *word, int k, int command_type)
{
	if (!trie || !word)
		return;

	int ok = 0, h_f_w_value = 0;
	char *search, *shortest_word, *first_in_dict, *highest_freq_word;
	// Alocare memorie pentru cuvântul curent
	search = (char *)malloc(sizeof(char) * (MAX_STRING_SIZE + 1));

	// initializam cuvintele cerute in enunt cu un sir de caractere
	// suficient de mare, pentru a putea fi inlocuite mai tarziu
	shortest_word = (char *)malloc(sizeof(char) * (MAX_STRING_SIZE + 1));
	strcpy(shortest_word, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	first_in_dict = (char *)malloc(sizeof(char) * (MAX_STRING_SIZE + 1));
	strcpy(first_in_dict, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	highest_freq_word = (char *)malloc(sizeof(char) * (MAX_STRING_SIZE + 1));
	strcpy(highest_freq_word, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	if (command_type == 0) {
		traverse_trie(trie, trie->root, search, word, 0, k, &ok,
					  command_type, shortest_word, first_in_dict,
					  highest_freq_word, &h_f_w_value, 0);
	} else if (command_type == 1) {
		trie_node_t *node = trie->root;
		for (int i = 0; i < (int)strlen(word); i++) {
			int index = word[i] - 'a';
			if (!node->children[index])
				goto label;
			node = node->children[index];
		}
		strcpy(search, word);
		traverse_trie(trie, node, search, word, strlen(word), k, &ok,
					  command_type, shortest_word, first_in_dict,
					  highest_freq_word, &h_f_w_value, 0);
	}

label:

	free(search);
	// daca nu am gasit niciun cuvant in trie, afisam mesajul corespunzator
	if (ok == 0) {
		printf("No words found\n");
		if (command_type == 1 && k == 0) {
			printf("No words found\n");
			printf("No words found\n");
		}
	} else if (command_type == 1) {
		if (k == 0) {
			if (strcmp(first_in_dict, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx") == 0)
				printf("No words found\n");
			else
				printf("%s\n", first_in_dict);
			if (strcmp(shortest_word, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx") == 0)
				printf("No words found\n");
			else
				printf("%s\n", shortest_word);
			if (strcmp(highest_freq_word, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx") == 0)
				printf("No words found\n");
			else
				printf("%s\n", highest_freq_word);
		} else if (k == 1) {
			if (strcmp(first_in_dict, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx") == 0)
				printf("No words found\n");
			else
				printf("%s\n", first_in_dict);
		} else if (k == 2) {
			if (strcmp(shortest_word, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx") == 0)
				printf("No words found\n");
			else
				printf("%s\n", shortest_word);
		} else if (k == 3) {
			printf("%s\n", highest_freq_word);
		}
	}
	free(shortest_word);
	free(first_in_dict);
	free(highest_freq_word);
}

// functia load citeste cuvinte dintr-un fisier text si le pune in trie
// folosind functia trie_insert
void load(trie_t *trie, char *file_name)
{
	FILE *file = fopen(file_name, "r");
	char *word = malloc(MAX_STRING_SIZE);

	while (fscanf(file, "%s", word) != EOF)
		trie_insert(trie, word);

	free(word);
	fclose(file);
}

int main(void)
{
	char *command = malloc(MAX_STRING_SIZE);
	trie_t *trie = trie_create(sizeof(int), ALPHABET_SIZE, ALPHABET);

	while (1) {
		scanf("%s", command);

		if (strcmp(command, "LOAD") == 0) {
			scanf("%s", command);
			load(trie, command);
		} else if (strcmp(command, "INSERT") == 0) {
			scanf("%s", command);
			trie_insert(trie, command);
		} else if (strcmp(command, "REMOVE") == 0) {
			scanf("%s", command);
			trie_remove(trie, command);
		} else if (strcmp(command, "AUTOCORRECT") == 0) {
			scanf("%s", command);
			int k;
			scanf("%d", &k);
			correct_or_complete(trie, command, k, 0);
		} else if (strcmp(command, "AUTOCOMPLETE") == 0) {
			scanf("%s", command);
			int k;
			scanf("%d", &k);
			correct_or_complete(trie, command, k, 1);
		} else if (strcmp(command, "EXIT") == 0) {
			trie_free(&trie);
			break;
		}
	}
	free(command);

	return 0;
}
