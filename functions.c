// Copyright 2023 Toma-Ioan Dumitrescu

#include "utils.h"

/* resets the dimensions of the main arrays
 * important to use the addresses of m, n and v as parameters
 */
void change_env(int **m, int **n, int ****v, int *act_size, int index,
				int *count_freed)
{
	if (index + 1 >= *act_size) {
		(*act_size) = (*act_size) * 2;

		int ***p1;
		p1 = (int ***)realloc(*v, (*act_size) * sizeof(int **));
		DIE(!p1, REALLOC_ERROR);
		*v = p1;

		int *p2;
		p2 = (int *)realloc(*m, (*act_size) * sizeof(int));
		DIE(!p2, REALLOC_ERROR);
		*m = p2;

		int *p3;
		p3 = (int *)realloc(*n, (*act_size) * sizeof(int));
		DIE(!p3, REALLOC_ERROR);
		*n = p3;
	} else if (*count_freed >= LOWER_BOUND) {
		/* Another option is to verify compared to act_size * coeff
		 * where coeff is subunitary
		 */
		(*act_size) -= (*count_freed);
		*count_freed = 0;

		int ***p1;
		p1 = (int ***)realloc(*v, (*act_size) * sizeof(int **));
		DIE(!p1, REALLOC_ERROR);
		*v = p1;

		int *p2;
		p2 = (int *)realloc(*m, (*act_size) * sizeof(int));
		DIE(!p2, REALLOC_ERROR);
		*m = p2;

		int *p3;
		p3 = (int *)realloc(*n, (*act_size) * sizeof(int));
		DIE(!p3, REALLOC_ERROR);
		*n = p3;
	}
}

// allocates a matrix on heap memory
int **create_matrix(int r, int c)
{
	int **new = calloc(r, sizeof(int *));
	DIE(!new, CALLOC_ERROR);

	for (int i = 0; i < r; i++) {
		new[i] = calloc(c, sizeof(int));
		DIE(!new[i], CALLOC_ERROR);
	}
	return new;
}

// deletes a matrix from heap
void free_matrix(int r, int **old)
{
	for (int i = 0; i < r; i++)
		free(old[i]);
	free(old);
}

// creates a matrix and assigns it to the matrix sequence
void load_matrix(int r, int c, int *m, int *n, int ***v, int *index)
{
	fscanf(stdin, "%d%d", &r, &c);
	int **new = create_matrix(r, c);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			fscanf(stdin, "%d", &new[i][j]);

	v[*index] = new;
	m[*index] = r, n[*index] = c;
	(*index)++;
}

// writes on stdout the dimensions of the matrix with the given index
void print_dimensions(int search_index, int *m, int *n, int index)
{
	fscanf(stdin, "%d", &search_index);
	if (search_index >= index || search_index < 0)
		fprintf(stdout, "No matrix with the given index\n");
	else
		fprintf(stdout, "%d %d\n", m[search_index], n[search_index]);
}

// prints the search_index matrix if it exists
void print_matrix(int search_index, int *m, int *n, int index, int ***v)
{
	fscanf(stdin, "%d", &search_index);
	if (search_index >= index || search_index < 0) {
		fprintf(stdout, "No matrix with the given index\n");
	} else {
		int a = m[search_index], b = n[search_index];
		for (int i = 0; i < a; i++) {
			for (int j = 0; j < b; j++)
				fprintf(stdout, "%d ", v[search_index][i][j]);
			fprintf(stdout, "\n");
		}
	}
}

// resizes a matrix (intersections between rows and columns)
void resize(int search_index, int *m, int *n, int index, int ***v)
{
	fscanf(stdin, "%d", &search_index);
	if (search_index >= index || search_index < 0) {
		// still reading the data
		int junk, knuj;

		fscanf(stdin, "%d", &junk);
		for (int i = 0; i < junk; i++)
			fscanf(stdin, "%d", &knuj);

		fscanf(stdin, "%d", &junk);
		for (int i = 0; i < junk; i++)
			fscanf(stdin, "%d", &knuj);

		fprintf(stdout, "No matrix with the given index\n");
	} else {
		int l, c, *l_arr, *c_arr;

		// reading rows
		fscanf(stdin, "%d", &l);
		l_arr = calloc(l, sizeof(int));
		DIE(!l_arr, CALLOC_ERROR);

		for (int i = 0; i < l; i++)
			fscanf(stdin, "%d", &l_arr[i]);

		// reading columns
		fscanf(stdin, "%d", &c);
		c_arr = calloc(c, sizeof(int));
		DIE(!c_arr, CALLOC_ERROR);

		for (int i = 0; i < c; i++)
			fscanf(stdin, "%d", &c_arr[i]);

		// resize math function applied to new
		int **new = create_matrix(l, c);
		for (int i = 0; i < l; i++)
			for (int j = 0; j < c; j++)
				new[i][j] = v[search_index][l_arr[i]][c_arr[j]];

		// pointers swap (v not retaining matrices, just the starting addresses)
		int **trash_mat = v[search_index];
		v[search_index] = new;
		free_matrix(m[search_index], trash_mat);

		// update the dimensions
		m[search_index] = l, n[search_index] = c;

		free(l_arr), free(c_arr);
	}
}

// function for matrix multiplication with parameter type of return
void mat_dot_pow(int **mat1, int **mat2, int **res, int m, int n, int p)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < p; j++) {
			int s = 0;
			for (int k = 0; k < n; k++) {
				s += mat1[i][k] * mat2[k][j];
				s %= OVERFLOW;
				if (s < 0)
					s += OVERFLOW;
			}
			res[i][j] = s;
		}
}

// calculates the product of 2 matrices if possible
void mat_product(int index1, int index2, int *m, int *n, int *index, int ***v)
{
	fscanf(stdin, "%d%d", &index1, &index2);
	if (index1 < 0 || index1 >= *index || index2 < 0 || index2 >= *index) {
		fprintf(stdout, "No matrix with the given index\n");
		return;
	}
	if (n[index1] != m[index2]) {
		fprintf(stdout, "Cannot perform matrix multiplication\n");
		return;
	}

	int **prod = create_matrix(m[index1], n[index2]);

	// O(dim1 * dim2) math algorithm for matrix product
	mat_dot_pow(v[index1], v[index2], prod, m[index1], n[index1], n[index2]);

	// index already incremented from load function
	v[*index] = prod;
	m[*index] = m[index1], n[*index] = n[index2];
	(*index)++;
}

// sorts the matrices increasingly with sum criterium
void sort_matrices(int index, int *m, int *n, int ***v)
{
	if (index <= 1)
		return;

	int *sum_arr = calloc(index, sizeof(int));
	DIE(!sum_arr, CALLOC_ERROR);

	for (int i = 0; i < index; i++) {
		for (int j = 0; j < m[i]; j++)
			for (int k = 0; k < n[i]; k++) {
				sum_arr[i] += v[i][j][k];
				sum_arr[i] %= OVERFLOW;
				if (sum_arr[i] < 0)
					sum_arr[i] += OVERFLOW;
			}
	}

	// selection sort
	for (int i = 0; i < index - 1; i++)
		for (int j = i + 1; j < index; j++) {
			if (sum_arr[i] > sum_arr[j]) {
				// pointers swap (matrices)
				int **tmp = v[i];
				v[i] = v[j];
				v[j] = tmp;

				// dimensions
				int aux = m[i];
				m[i] = m[j];
				m[j] = aux;

				aux = n[i];
				n[i] = n[j];
				n[j] = aux;

				// sums to register the swap
				aux = sum_arr[i];
				sum_arr[i] = sum_arr[j];
				sum_arr[j] = aux;
			}
		}

	free(sum_arr);
}

// calculates the transpose of a matrix (aij) -> (aji)
void transpose(int search_index, int index, int *m, int *n, int ***v)
{
	fscanf(stdin, "%d", &search_index);
	if (search_index >= index || search_index < 0) {
		fprintf(stdout, "No matrix with the given index\n");
		return;
	}

	int **new = create_matrix(n[search_index], m[search_index]);

	for (int i = 0; i < n[search_index]; i++)
		for (int j = 0; j < m[search_index]; j++)
			new[i][j] = v[search_index][j][i];

	// pointers swap trick to free the memory
	int **trash_mat = v[search_index];
	v[search_index] = new;
	free_matrix(m[search_index], trash_mat);

	// update dimensions
	int aux = m[search_index];
	m[search_index] = n[search_index];
	n[search_index] = aux;
}

// dest will have the values of src matrix
void copy_matrix(int **dest, int **src, int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			dest[i][j] = src[i][j];
}

// recursive function for raising a matrix to p-th power
void power_matrix(int search_index, int p, int dim, int ***v,
				  int **sol, int **original, int **buffer)
{
	if (!(p > 1))
		return;

	/* buffer_matrix prevents mat_dow_pow function from modifying
	 * some parameters while doing the arithmetical operations
	 * especially in the form result = result * some_matrix
	 * as result cannot be passed as a product parameter (only
	 * as a result parameter)
	 */

	if (p % 2) {
		// calculating side A^k
		copy_matrix(buffer, sol, dim);
		mat_dot_pow(buffer, original, sol, dim, dim, dim);

		// calculating act_matrix squared
		copy_matrix(buffer, v[search_index], dim);
		mat_dot_pow(buffer, buffer, v[search_index], dim, dim, dim);

		// squared exponentiation (parameter is act_matrix squared)
		copy_matrix(original, v[search_index], dim);
		power_matrix(search_index, (p - 1) / 2, dim, v, sol, original, buffer);
	} else {
		// calculating act_matrix squared
		copy_matrix(buffer, v[search_index], dim);
		mat_dot_pow(buffer, buffer, v[search_index], dim, dim, dim);

		// squared exponentiation (parameter is act_matrix squared)
		copy_matrix(original, v[search_index], dim);
		power_matrix(search_index, p / 2, dim, v, sol, original, buffer);
	}
}

// raising the matrix to power p in (n^2)log2(p) complexity
void log2_power_main(int search_index, int p, int index,
					 int *m, int *n, int ***v)
{
	fscanf(stdin, "%d%d", &search_index, &p);
	if (search_index >= index || search_index < 0) {
		fprintf(stdout, "No matrix with the given index\n");
		return;
	}
	if (p < 0) {
		fprintf(stdout, "Power should be positive\n");
		return;
	}
	if (m[search_index] != n[search_index]) {
		fprintf(stdout, "Cannot perform matrix multiplication\n");
		return;
	}

	if (p == 0) {
		for (int i = 0; i < m[search_index]; i++)
			for (int j = 0; j < n[search_index]; j++) {
				if (i == j)
					v[search_index][i][j] = 1;
				else
					v[search_index][i][j] = 0;
			}
		return;
	}

	if (p == 1)
		return;

	// auxiliary matrices
	int **solution = create_matrix(m[search_index], n[search_index]);
	int **original = create_matrix(m[search_index], n[search_index]);
	int **buffer = create_matrix(m[search_index], n[search_index]);

	for (int i = 0; i < m[search_index]; i++)
		for (int j = 0; j < n[search_index]; j++) {
			original[i][j] = v[search_index][i][j];
			if (i == j)
				solution[i][j] = 1;
			else
				solution[i][j] = 0;
		}

	power_matrix(search_index, p, m[search_index], v,
				 solution, original, buffer);

	/* we multiply v[search] with solution, because solution
	 * holds side matrices product appearing in odd power cases
	 * we also use the fact that matrix product is commutative
	 * in (S={A^k | k positive integer}, *).
	 */

	copy_matrix(buffer, v[search_index], m[search_index]);
	mat_dot_pow(buffer, solution, v[search_index], m[search_index],
				m[search_index], m[search_index]);

	free_matrix(m[search_index], solution);
	free_matrix(m[search_index], original);
	free_matrix(m[search_index], buffer);
}

// deletes a matrix from the array list
void delete_element(int search_index, int *m, int *n, int ***v,
					int *index, int *count_freed)
{
	fscanf(stdin, "%d", &search_index);
	if (search_index >= *index || search_index < 0) {
		fprintf(stdout, "No matrix with the given index\n");
		return;
	}

	int **trash_mat = v[search_index], r = m[search_index];

	// array deleting element algorithm
	for (int i = search_index; i < *index - 1; i++) {
		v[i] = v[i + 1];
		m[i] = m[i + 1];
		n[i] = n[i + 1];
	}

	free_matrix(r, trash_mat);

	(*index)--;
	(*count_freed)++;
}

// clearing env variables
void clear_memory(int *m, int *n, int ***v, int index)
{
	for (int i = 0; i < index; i++)
		free_matrix(m[i], v[i]);

	free(v);
	free(m);
	free(n);
}
