#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <ctype.h>   
#include "trie1.h"

//new_node constructs a new TRIE node
//Parameters: A pointer to a string representing the definition to be assigned to the new node
//Returns: Pointer to TRIE struct
TRIE *new_node(char *definition) {
	
	const char *default_def = "\0";
	
	TRIE *np = (TRIE *) malloc(sizeof(TRIE));
	if (np == NULL) return NULL;

	np->is_end_word = 0;

	if(definition == NULL){		//If no definition is provided, the definition is set to the default "\0"
		strncpy(np->definition, default_def, 50);
	}

	if(definition){
		strncpy(np->definition, definition, 49);
		np->definition[50] = '\0';
	}

	for(int i = 0; i<ALPHABET_LENGTH; i++){
		np->children[i] = NULL;
	}
	return np;
}


//is_parent determines if a given TRIE node has any children
//Parameters: a pointer to a TRIE node
//Returns: an integer; 1 if given node is a parent or 0 if the node is a leaf
int is_parent(TRIE *current){
	for(int i = 0; i < ALPHABET_LENGTH; i++){
		if(current->children[i] != NULL){
			return 1;
		}
	}
	
	return 0;
}

//lowercase_string converts a given string to all lowercase characters
//Parameters: a string
//Returns: none
void lowercase_string(char str[]){
	for(int i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
}

//validate_definition ensures that any new definition is valid before applying it to the trie structure
//Parameters: a pointer to the string containing the proposed definition
//Returns: none
void validate_definition(char *definition){
	
	int end_reached = 0;	//this flag keeps track of whether or not the end of the definition has been reached
	int valid_def = 1;
	int only_whitespace = 1;	//this flag keeps track of whether or not any non-whitespace characters have been reached
	
	for(int i = 0; i < 50 && end_reached == 0 && valid_def == 1; i++){
		if(definition[i] == '\0'){
			end_reached = 1;
			if(only_whitespace == 1){	//a definition is not valid if it only contains whitespace
				valid_def = 0;
			}
		}
		
		else if(isspace(definition[i]) == 0 && isalpha(definition[i]) == 0 && definition[i] != 39 && definition[i] != 44){	//a definition is valid as long as it only contains spaces, letters, apostrophes and commas
			valid_def = 0;
		}
		
		else if(isspace(definition[i]) == 0){
			only_whitespace = 0;
		}
	}

	fflush(stdin);
		
 	while(valid_def == 0){	//if an invalid definition was entered, the user is continuously prompted to retry until a valid definition is entered
		printf("Sorry, you entered an invalid definition! Please try again:\n");
		if(fgets(definition, 50, stdin) != NULL){		
			validate_definition(definition); 
			valid_def = 1;
			end_reached = 0;
			only_whitespace = 1;
			
			for(int i = 0; i < 50 && end_reached == 0 && valid_def == 1; i++){
				if(definition[i] == '\0'){
					end_reached = 1;
					if(only_whitespace == 1){
						valid_def = 0;
					}
				}
				
				else if(isspace(definition[i]) == 0 && isalpha(definition[i]) == 0 && definition[i] != 39 && definition[i] != 44){
					valid_def = 0;
				}
				
				else if(isspace(definition[i]) == 0){
					only_whitespace = 0;
				}
			}
			fflush(stdin); 
		}
	}
}	


//this function inserts the necessary TRIE node(s) into an existing trie such that the given word is added to the dictionary 
//Parameters: a pointer to the root node of the trie, a pointer to a string holding the word to be inserted, a pointer to a string holding the definition of the new word
//Returns: none
void insert(TRIE *root, char *word, const char *definition){
	
	TRIE *current = root;

	TRIE *search_results = search(root, word);	//check if the word is already in the trie
	
	if(search_results != NULL){		//if the word already exists, the user has the choice to update the definition or leave it as is
		printf("Looks like this word is already in the dictionary! Would you like to update it's definition?\n");
		printf("Enter 1 for yes or 0 for no...\n");
		
		int answer;
		int valid_answer = 0;
		
		if(scanf("%1d", &answer) && (answer == 0 || answer == 1)){
			valid_answer = 1;
		}
		
		while(valid_answer == 0){
			printf("Please enter either 1 or 0...\n");
			if(scanf("%1d", &answer) && (answer == 0 || answer == 1)){
				valid_answer = 1;
			}
		}
		
		fflush(stdin);
		
		if(answer == 1){
			modify_def(root, word);
		}
		return;
	}

	else{
		while(*word){	//iterate through each letter of the new word and check if the required node path already exists 
			
			if(current->children[*word - 'a'] == NULL){		//create a new node if the path doesn't exist	
				current->children[*word - 'a'] = new_node(NULL);	
			}
			
			current = current->children[*word - 'a'];				
			word++;	
		}
		
		strncpy(current->definition, definition, 49);
		current->is_end_word = 1;	
		
		printf("Word added!\n");
	}
}

//delete_word uses recursion to delete a word from the trie
//Parameters: A double pointer to the current node, a double pointer to the root of the trie, a pointer to a string holding the word to be deleted
//Returns: integer (1 if successful, 0 if done/failed)
int delete_word(TRIE **current, TRIE **root, char *word){
	
	if(*current == NULL){	//return if trie is empty
		return 0;
	}
	
	//if the end of the string has not been reached
	if(*word){
		//use recursion to get to the trie node corresponding to the next character in the word
		if(*current != NULL && (*current)->children[*word - 'a'] != NULL && delete_word(&((*current)->children[*word - 'a']), root, word + 1) && (*current)->is_end_word == 0){
			
			if(!is_parent(*current) && (*current != *root)){	//if the node has no children and it is not the root of the trie, it is deleted
				free(*current);
				(*current) = NULL;								
				return 1;
			}
			
			else{
				return 0;
			}
		}
	}
	
	if(*word == '\0' && (*current)->is_end_word == 1){		//if we have reached the end of the string
		
		if(!is_parent(*current)){	//if the node has no children it is deleted
			free(*current);
			(*current) = NULL;
			return 1;
		}
		
		else{
			(*current)->is_end_word = 0; 	//if the node representing the end of the word and is not a leaf, simply tag it as NOT the end of a word thus removing its dictionary functionality
			return 1;
		}
	}
	
	return 0;	
}
	

//display_content outputs an alphabetically ordered list of all words and definitions currently stored in the trie 
//Parameters: a pointer to the parent node, a string to hold the current word to be printed, an integer to keep track of the current level of the trie/index of the string 
//Returns: none
void display_content(TRIE *parent, char str[], int level){

	if (parent->is_end_word == 1){ 	//if we have reached the end of a word, output the word and the corresponding definition	
		str[level] = '\0';
		printf("%-25s%s\n", str, parent->definition);
	} 
	
	int i; 
	for (i = 0; i < ALPHABET_LENGTH; i++){	

		if (parent->children[i]){  //if non-null child is found, add the alphabetical equivalent of that child's index to the string and use recursion to get the rest of the word 		 
			int k = i + 97;
			str[level] = (char) k;
			display_content(parent->children[i], str, level + 1); 
		} 
	} 
} 

//search is an iterative function which searches for a given word in the trie
//Parameters: a pointer to the root of the trie, a pointer to a string containing the word to be searched for
//Returns: a pointer to the the last trie node in the path representing the word
TRIE* search(TRIE *root, char *search_key){
	
	TRIE *current = root;
	char *key = search_key;
	
	if(root == NULL){
		return NULL;
	}
	
	while(*key){	//iterate through each character in the string

		current = current->children[*key - 'a'];	//the current node is the child whose index equates to the current letter in the string
		
		if(current == NULL){
			return NULL;
		}
		
		key++;
	}
	
	if(current->is_end_word == 0){		//if the last letter of the searched word is marked as NOT the end of a word, this means the word has previously been deleted and must not return any search results
		return NULL;
	}
	
	else{	
		printf("Here are the search results for %s:\n%-20s%s\n", search_key, search_key, current->definition);
		return current;
	}

}

//modify_def searches for a given word in the trie and if found, prompts the user to enter an updated definition for that word and updates the trie accordingly
//Parameters: a pointer to the root node of the trie, a pointer to a string containing the word for which the definition is to be updated
//Returns: none
void modify_def(TRIE *root, char *search_key){
	
	TRIE *search_results = search(root, search_key);
	
	if(search_results == NULL){
		printf("Could not update definition because the word could not be found.\n");
		return;
	}
	
	else{
		printf("What would you like to change the definition to? (Max. 49 characters)\n");
		
		if(fgets(search_results->definition, 50, stdin) != NULL){
			validate_definition(search_results->definition);
			lowercase_string(search_results->definition);	
			printf("Definition now updated to: %s\n", search_results->definition);
		}
		
		fflush(stdin);		
	}

}

