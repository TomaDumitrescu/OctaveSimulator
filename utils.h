// Copyright 2023 Toma-Ioan Dumitrescu

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// shortcuts
#define CALLOC_ERROR "Calloc didn't work as expected!\n"
#define REALLOC_ERROR "Realloc didn't work as expected!\n"
#define OVERFLOW 10007
#define START_SIZE 10
#define LOWER_BOUND 5

// macro for defensive programming
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

// functions headers
void change_env(int **m, int **n, int ****v, int *act_size, int index,
				int *count_freed);
void load_matrix(int r, int c, int *m, int *n, int ***v, int *index);
void print_dimensions(int search_index, int *m, int *n, int index);
void print_matrix(int search_index, int *m, int *n, int index, int ***v);
void resize(int search_index, int *m, int *n, int index, int ***v);
void mat_product(int index1, int index2, int *m, int *n, int *index, int ***v);
void sort_matrices(int index, int *m, int *n, int ***v);
void transpose(int search_index, int index, int *m, int *n, int ***v);
void log2_power_main(int search_index, int p, int index,
					 int *m, int *n, int ***v);
void delete_element(int search_index, int *m, int *n, int ***v,
					int *index, int *count_freed);
void clear_memory(int *m, int *n, int ***v, int index);

#endif
