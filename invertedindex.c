// COMP2521 Assignment 1 Term 1 2022
// Written by Mann Arora z5360526 on 27 March 2022
// This assignment involves the use of binary search trees and linked list. Given an inverted index, we are asked to search, create and 
// insert word nodes that each have a filelist connected to them. The filelist contains the names of files that contain that specific word.
// Moreover they contain the term frequency value of that word within that specific file. 

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
//                                      PART 1                                      //
//////////////////////////////////////////////////////////////////////////////////////

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
	// Create an empty Binary Search Tree  
	struct InvertedIndexNode *inverted_index = NULL; 
	// Create file list with collectionFilename
	struct FileNode *new_list = create_file_list(collectionFilename);  
	FileList curr_file_list = new_list;
	// Malloc word
	char *word = malloc(sizeof(char) * MAX);
	// Iterate through files in file list
	while (curr_file_list != NULL) {    
		// open the current file in list in reading mode
		FILE *file = fopen(curr_file_list->filename, "r");
		int word_freq = 0;
		// scan every string in current file till end of file
		while (fscanf(file, "%s", word) != EOF) {
			// Normalise the string into word
			word = normalise_word(word);
			// search for the current word node in the Binary Search Tree
			struct InvertedIndexNode *curr_word = search_bst_node(inverted_index, word);
			// if the node doesnt exist, create and insert the word node
			if (word != NULL) {
				if (curr_word == NULL) {
                	curr_word = create_bst_node(new_list, word);
                	inverted_index = insert_bst_node(inverted_index, curr_word);
            	}
			}
            // search for list node file name
            struct FileNode *curr_file = search_list_node(curr_file_list->filename,curr_word->fileList);
			// Increaset tf by 1 
            curr_file->tf++;
            word_freq++;
		}
		// Obtain Tf value of current file
		tf_value(inverted_index, curr_file_list->filename, word_freq);
		fclose(file);
		// Iterate again to next FileList
		curr_file_list = curr_file_list->next;
	}
	// Free word - avoid memory leak
	free(word);
	return inverted_index;
}

 
void printInvertedIndex(InvertedIndexBST tree, char *filename) {
	// open given file in write mode
	FILE *BST = fopen(filename, "w");
	// print the word, tf value and the filenames
	print_inverted_index(tree, BST);
	fclose(BST);
}

void freeInvertedIndex(InvertedIndexBST tree) {
	// Free the Binary Search Tree nodes and the filelist nodes
	free_BST(tree);
	
}

//////////////////////////////////////////////////////////////////////////////////////
//                                      PART 2                                      //
//////////////////////////////////////////////////////////////////////////////////////

TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
	// Create Inverted Index Node for the found word
	struct InvertedIndexNode *found_word = search_bst_node(tree, searchWord);
	// create a empty filel ist
	struct TfIdfNode *list = NULL;
	// Calculate idf value
	double idf = calculateIdf(found_word->fileList, D);
	double tfidf;
	FileList curr = found_word->fileList;
	// Iterate through file list attached to the node of the found word
	while (curr != NULL) {
		// obtain tfidf value
		tfidf = curr->tf * idf;
		if (tfidf > 0 || tfidf < 0) {
			// insert the tfidf value in the list for the files
			list = insert_tfidf_list_node(list, curr->filename, tfidf);
		}
		// iterate to next file
		curr = curr->next;
	}
	// return list of files 
	return list;
}

TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
	// Create empty TfIdfList 
	TfIdfList list = NULL;
	// Iterate through searchWords
    for (int i = 0; searchWords[i] != NULL; i++) {
		// new list that gets list of files containing the specified word searchWords[i]
		TfIdfList new = searchOne(tree, searchWords[i], D);
		// If list is empty then make the list as one
		if (list == NULL) {
			list = new;
		}
		// If list is not empty, append new on list
		else {
			list = join_two_tfidf_lists(list, new);
		}
	}
	// Delete duplicate file nodes within the list
	delete_same_nodes(list);
	// Order the files
	list = order_tfidf_list(list);
	// return list
	return list;
}

void freeTfIdfList(TfIdfList list) {
	// Free list nodes
	free_tfidf_nodes(list);
}










