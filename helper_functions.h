// COMP2521 Assignment 1 Term 1 2022
// Written by Mann Arora z5360526 on 27 March 2022
// This file contains the prototype functions for helper functions for invertedIndex.c

#include "invertedIndex.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ------------------ HELPER FUNCTION PROTOTYPES ------------------------


// ------------ Part 1 ------------
char *normalise_word(char *str);
FileList create_list_node(char *filename, FileList new_file_list);
FileList insert_list_node(char *filename, FileList fileList);
FileList create_file_list(char *collectionFilename);
FileList copy_file_list(FileList fileList);
InvertedIndexBST create_bst_node(FileList fileList, char *word);
char *copy_string (char *string);
InvertedIndexBST insert_bst_node(InvertedIndexBST inverted_index, InvertedIndexBST invertedIndexNode);
InvertedIndexBST search_bst_node(InvertedIndexBST inverted_index, char *word);
FileList search_list_node(char *filename, FileList fileList);
void tf_value(InvertedIndexBST inverted_index, char *filename, int word_freq);
void print_tf_value(FileList file_list, FILE *result);
void print_inverted_index(InvertedIndexBST inverted, FILE *result);
void free_BST(InvertedIndexBST tree);
void free_list_nodes(FileList node);


// ------------ Part 2 ------------
double calculateIdf(FileList fileList, int x);
TfIdfList create_tfidf_list_node(char *filename, double tfidf, TfIdfList list);
TfIdfList  insert_tfidf_list_node(TfIdfList list, char *filename, double tfidf);
TfIdfList join_two_tfidf_lists(TfIdfList list1, TfIdfList list2);
void delete_same_nodes(TfIdfList node);
TfIdfList order_tfidf_list(TfIdfList node);
void free_tfidf_nodes(TfIdfList list);
