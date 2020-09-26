#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   

#define ALPHABET_LENGTH 26

typedef struct Trie{
	struct Trie *children[ALPHABET_LENGTH];
	char definition[50];
	int is_end_word;	
}TRIE;

TRIE *new_node(char *definition);

int is_parent(TRIE *current);

void lowercase_string(char str[]);

void validate_definition(char *definition);

void insert(TRIE *root, char* word, const char *definition);

int delete_word(TRIE **current, TRIE **root, char *word);

void display_content(TRIE *parent, char str[], int level);

TRIE *search(TRIE *root, char *search_key);

void modify_def(TRIE *root, char *search_key);

