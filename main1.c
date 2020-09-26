#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>       
#include "trie1.h"

void main(){
	
	TRIE *root = new_node(NULL);
	int input;
	int error_flag;

	printf("WELCOME TO JANELLE'S DICTIONARY\n");
	
	printf("\nWhat would you like to do? (Enter the number of the action and press the ENTER key)\n");
	printf("(1) Display entire dictionary\n");
	printf("(2) Search for a word\n");
	printf("(3) Add a new word\n");
	printf("(4) Remove a word\n");
	printf("(5) Edit a definition\n");
	printf("(6) Quit\n\n");
	
	error_flag = scanf("%d", &input);
	fflush(stdin);

	
	while(error_flag == 1){
		
		if(input == 1){		
			
			if(is_parent(root) == 0){
				printf("Dictionary is empty! Nothing to display.\n");
			}
			
			else{
				char str[20];				
				printf("\n\n%-25s%s\n\n", "WORD", "DEFINITION");
				display_content(root, str, 0);
			}
		}
		
		else if(input == 2){
			printf("Enter the word you're looking for...\n");
			
			char search_key[20];
			scanf("%20s", &search_key);
			fflush(stdin);
			
			lowercase_string(search_key);
			
			TRIE *search_result = search(root, search_key);
			
			if(!search_result){
				printf("Sorry, that word is not currently in the dictionary.\n");
			}
			
		}
		
		else if(input == 3){
			char word[20];
			char definition[50];
			int valid_word = 1;
			
			for(int i = 0; i < 20; i++){
				word[i] = '\0';
			}
			
			printf("Enter the word you would like to add...\n");
			
			scanf("%20s", &word);
			word[19] = '\0';
			fflush(stdin);
			
			lowercase_string(word);	
			
			for(int i = 0; i < 20 && valid_word == 1; i++){
				if((isalpha(word[i]) == 0) && word[i] != 0 && word[i] != 39 && word[i] != 92){
					valid_word = 0;
				}
			}	
			
			while(valid_word == 0){
				printf("Sorry, you entered an invalid word! Please try again:\n");
				scanf("%20s", &word);
				fflush(stdin);				
				valid_word = 1;
				
				for(int i = 0; i < 20 && valid_word == 1; i++){		
					if(isalpha(word[i]) == 0 && word[i] != 0 && word[i] != 39){
						valid_word = 0;
					}
				}
			}		

			fflush(stdin);
			
			printf("Enter the definition of this word (Max. 49 characters)...\n");

			if(fgets(definition, 50, stdin) != NULL){							
				validate_definition(definition);	
				lowercase_string(definition);			
				insert(root, word, definition);					
			}		
			fflush(stdin);
					
		}
	
		
		else if(input == 4){
			
			char word[20];
			printf("Enter the word you would like to delete...\n");
			scanf("%20s", &word);
			fflush(stdin);
			lowercase_string(word);
			
			TRIE *search_results = search(root, word);
			
			if(!search_results){
				printf("That word could not be found in the dictionary!\n");
			}
			
			else{
				delete_word(&root, &root, word);
				printf("Deleted!\n");
			}
		}
		
		else if(input == 5){
			
			char word[20];
			printf("Enter the word you would like to modify the definition of...\n");
			scanf("%20s", &word);
			fflush(stdin);
			
			lowercase_string(word);
			modify_def(root, word);
		}
		
		else if(input == 6){
			printf("Goodbye!\n");
			break;
		}
		
		else{
			printf("Invalid input! Please select an option below...\n");
		}
		
		printf("\n\nWhat would you like to do? (Enter the number of the action and press the ENTER key)\n");
		printf("(1) Display entire dictionary\n");
		printf("(2) Search for a word\n");
		printf("(3) Add a new word\n");
		printf("(4) Remove a word\n");
		printf("(5) Edit a definition\n");
		printf("(6) Quit\n\n");
		
		error_flag = scanf("%d", &input);
		fflush(stdin);
				
	}
	
	if(error_flag != 1){
		printf("Program terminated because of user input error.\n");
	}
	
}