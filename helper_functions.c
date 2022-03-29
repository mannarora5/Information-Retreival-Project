// COMP2521 Assignment 1 Term 1 2022
// Written by Mann Arora z5360526 on 27 March 2022
// This file contains the helper function definitions for invertedIndex.c

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "invertedIndex.h"
#include "helper_functions.h"
#define MAX 100


//////////////////////////////////////////////////////////////////////////////////////
//                                  HELPER FUNCTIONS                                //
//////////////////////////////////////////////////////////////////////////////////////




// ------------------------- Part 1 Helper Functions ------------------------- 


// Normalises a string into word
char *normalise_word(char *str) {
	int len = strlen(str);
	int index = 0;
	// remove space before char argument input 
	while (str[index] == ' ') {
		while (str[index]) {
			// assign first index as next
			str[index] = str[index + 1];
			// check next index
			index++;
		}
		// after each iteration, decrease string length by 1 (remove spaces)
		len = len - 1;
	}
	index = 0;
	int temp = 0;
	// remove punctuation at end of word
	while (index < len) {
		// if str[index + 1] == space, then check for str[index] being a punctuation
		if (str[index+1] == '\n' || str[index+1] == '\0' || str[index+1] == ' ' || str[index+1] == '\t') {
			if (str[index] == '.' || str[index] == ',' || str[index] == ':' || str[index] == ';' || str[index] == '?' || str[index] =='*') {
				temp = index;
				while (str[temp]) {
					str[temp] = str[temp + 1];
					temp++;
				}
			}
		}	
		index++;
    }  
	// remove space left by punctuation
	index = len - 1;
	while (str[index] == ' ' && index >= 0) {
		temp = index;
		while (str[temp]) {
			str[temp] = str[temp + 1];
			temp++;
		}
		index--;
	}
	// make all the characters lowercase in word
	index = 0;
	while (index < len) {
		str[index] = tolower(str[index]);
		index++;
	}
	return str;
}

// Creates a file node for file list
FileList create_list_node(char *filename, FileList fileList) {
	// create a new node
	FileList new_node = malloc(sizeof(struct FileNode));
	// Assign new node with respective information (filename and tf)
	new_node->filename = copy_string(filename);
	new_node->tf = 0;
	new_node->next = fileList;
	return new_node;
}

// Insert file node for list in alphabetical order 
FileList insert_list_node(char *filename, FileList fileList) {
	
	// If filename is in order or filelist is NULL
	if (fileList == NULL || strcmp(fileList->filename, filename) > 0) {
		// Create a file node
		return create_list_node(filename, fileList);
	}
	// Go to the next node in list and recursively check when it fits in order
	fileList->next = insert_list_node(filename, fileList->next);
	return fileList;
}

// Create the list containing file nodes
FileList create_file_list(char *collectionFilename) {
	// Make new list empty
	FileList new_list = NULL;
	// Open the collection file in reading mode
	FILE *fileCollection = fopen(collectionFilename, "r");
	char *filename = malloc(sizeof(char) * 100);
	// read file name from collection file and insert to new list till EOF 
	while (fscanf(fileCollection, "%s", filename) != EOF) {
		new_list = insert_list_node(filename, new_list);
	}
	free(filename);
	fclose(fileCollection);
	// return the new list
	return new_list;
}

// Copy the file list
FileList copy_file_list(FileList fileList) {
    // if given list is empty
	if (fileList == NULL) {
		return NULL;
	}
    // create new list for file list
    FileList new_node = malloc(sizeof(struct FileNode));
    FileList new_list = new_node;
    new_node->filename = copy_string(fileList->filename);
    new_node->tf = fileList->tf;
    // Iterate and copy the filename and tf value into new list
    for (FileList curr = fileList->next; curr != NULL; curr = curr->next) {
        new_node->next = malloc(sizeof(struct FileNode));
        new_node->next->filename = copy_string(curr->filename);
        new_node->next->tf = curr->tf;
        new_node = new_node->next;
    }
    new_node->next = NULL;
    // return the new list
    return new_list;
}


// Create the BST Node 
InvertedIndexBST create_bst_node(FileList fileList, char *word) {
    // Malloc new node
	InvertedIndexBST new_node = malloc(sizeof(struct InvertedIndexNode));
    // Assign word
    new_node->word = copy_string(word);
    // Assign filelist
    new_node->fileList = copy_file_list(fileList);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Copies and malloc string and returns copied string
char *copy_string (char *string) {
	int length = strlen(string);
	char *copy = malloc(length + 1);
	if (copy != NULL) {
		strcpy(copy, string);
	}
	return copy;
}

// insert BST node in tree
InvertedIndexBST insert_bst_node(InvertedIndexBST inverted_index, InvertedIndexBST invertedIndexNode) {
    // if index is empty
	if (inverted_index == NULL) {
		return invertedIndexNode;
	}
    // Compare word in tree and given word to order and insert alphabetically
	int order = strcmp(inverted_index->word, invertedIndexNode->word);
	if (order > 0) {
        inverted_index->left = insert_bst_node(inverted_index->left, invertedIndexNode);
        return inverted_index;
	}
    if (order < 0) {
        inverted_index->right = insert_bst_node(inverted_index->right, invertedIndexNode);
        return inverted_index;
    } else {
        return inverted_index;
    }

}

// Search BST Node containg word
InvertedIndexBST search_bst_node(InvertedIndexBST inverted_index, char *word) {
	// If the inverted contains no nodes 
	if (inverted_index == NULL) {
		return NULL;
	} // If word < inverted_index->word search left side of BST
	else { 
		int compare = strcmp(inverted_index->word, word);
		if (compare > 0) {
			return search_bst_node(inverted_index->left, word);
		}
		// If word > inverted_index->word search right side of BST
		else if (compare < 0) {
			return search_bst_node(inverted_index->right, word);
		}
		// if equal return the node
		else {
			return inverted_index;
	} 
	
}
}
// Search for list node 
FileList search_list_node(char *filename, FileList fileList) {
    // if list is empty 
	if (fileList == NULL) {
		return NULL;
	}
    // if strcmp returns 0, this means list node has been found
	int compare = strcmp(fileList->filename, filename);
	if (compare == 0) {
		return fileList;
	}
    // Otherwise recursively search next list node
	else {
		return search_list_node(filename, fileList->next);
	}
}

// Calculates the term frequency (tf)
void tf_value(InvertedIndexBST inverted_index, char *filename, int word_freq) {
    // If inverted index exists and has nodes
    if (inverted_index != NULL) {
        tf_value(inverted_index->left, filename, word_freq);
        FileList currFile = search_list_node(filename, inverted_index->fileList);
        currFile->tf = currFile->tf/word_freq;
        tf_value(inverted_index->right, filename, word_freq);
    }
}

// Prints the tf value into output file
void print_tf_value(FileList file_list, FILE *file) {
    FileList temp = file_list;
    // while temp has not reached end of list
    while (temp != NULL) {
        if (temp->tf != 0) {
            // print tf value into file
            fprintf(file, "%s (%.7lf) ", temp->filename, temp->tf);
        }
        // go next file node
        temp = temp->next;
    }
    // newline 
    fprintf(file, "\n");
}

// print the inverted index
void print_inverted_index(InvertedIndexBST inverted_index, FILE *file) {
    // if inverted index is not empty
    if (inverted_index != NULL) {
        // go left and recursively print into file
        print_inverted_index(inverted_index->left, file);
        // print word into file
        fprintf(file, "%s ", inverted_index->word);
		// print tf value into file
        print_tf_value(inverted_index->fileList, file);
        // go right and recursively print into file
        print_inverted_index(inverted_index->right, file);
    }

}

// free the inverted index
void free_BST(InvertedIndexBST tree) {
	if (tree != NULL) {
        // recursively free left
		free_BST(tree->left);
        // free word
		free(tree->word);
        // free list nodes
		free_list_nodes(tree->fileList);
        // recursively free right 
		free_BST(tree->right);
        // free tree
		free(tree);
	}
}

// Free content from list nodes
void free_list_nodes(FileList node) {
    FileList curr = node;
    if (node != NULL) {
        // free filename
	    free(curr->filename);
        // recursively free next file
	    free_list_nodes(node->next);
        // free file
	    free(curr);
    }
}




// ------------------------- Part 2 Helper Functions ------------------------- 




// calculate a word's idf value from a given fileList
double calculateIdf(FileList fileList, int x) {
    double value = 0;
	FileList curr = fileList;
	while (curr != NULL) {
        // if tf is greater than 0 then +1 value
		if (curr->tf > 0) {
			value++;
		}
		curr = curr->next;
	}
    // Calc idf
    double idf = log10(x/value);
    return idf;
}

// create a tfidf node
TfIdfList create_tfidf_list_node(char *filename, double tfidf, TfIdfList list) {
    // make new node and assign filename and tfidf value
	TfIdfList new_node = malloc(sizeof(TfIdfList));
    new_node->tfIdfSum = tfidf;
	new_node->filename = copy_string(filename);
	new_node->next = list;
	return new_node;
}

// insert a tfidf node in correct order
TfIdfList insert_tfidf_list_node(TfIdfList list, char *filename, double tfidf) {
    // if list is empty of if the tfidf value is greater than the given nodes tfidf
	if (list == NULL || (list->tfIdfSum) < tfidf) {
        // create node with tfidf
		return create_tfidf_list_node(filename, tfidf, list);
	}
    // if tfidf value == given tfidf & its positive in strcmp
	else if (list->tfIdfSum == tfidf && (strcmp(list->filename, filename)) > 0) {
        // create node with tfidf
		return create_tfidf_list_node(filename, tfidf, list);
	}
    // recursively insert nodes in order
    list->next = insert_tfidf_list_node(list->next, filename, tfidf);
    return list;
}

// Combine two lists 
TfIdfList join_two_tfidf_lists(TfIdfList list1, TfIdfList list2) {
	TfIdfList previous = NULL;
	TfIdfList temp = list1;
	while (temp != NULL) {
		previous = temp;
		temp = temp->next;
	}
	previous->next = list2;
	return list1; 
}

// delete the same nodes within the list
void delete_same_nodes(TfIdfList node) {
    // Loop through nodes
	for (TfIdfList curr = node; curr != NULL; curr = curr->next) {
        // previous node is current
        TfIdfList previous = curr;
        TfIdfList temp = curr;
        TfIdfList next_node = temp;
        // if temp is not empty then compare
        while (temp != NULL) {
            if (strcmp(curr->filename, temp->filename) == 0){
                if (curr != temp){
                    curr->tfIdfSum += temp->tfIdfSum;
                    previous->next = next_node;
                    // free the duplicate
                    free(temp);
                    temp = next_node;
                    if (temp != NULL) {
                        next_node = temp->next;
                    }
                    continue;  
                }
            }
            previous = temp;
            // Go next in temp
            temp = temp->next;
            if (temp == NULL) {
                break;
            }
            // Go next node
            next_node = temp->next;
        }
    }
}

// order the list 
TfIdfList order_tfidf_list(TfIdfList node) {
    TfIdfList new = NULL;
    TfIdfList curr = node;
    // loop and insert node in order
    while (curr != NULL) {
        new = insert_tfidf_list_node(new, curr->filename, curr->tfIdfSum);
        curr = curr->next;
    }
    return new;
}

// Free up the list nodes
void free_tfidf_nodes(TfIdfList list) {
	// recursion
	TfIdfList temp = list;
	if (list != NULL) {
        // free filename
		free(list->filename);
		free_tfidf_nodes(temp->next);
        // free list
		free(list);
	}
}

