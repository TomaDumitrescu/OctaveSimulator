// Copyright 2023 Toma-Ioan Dumitrescu

#include "utils.h"

int main(void)
{
	char command;
	int ***v, *m, *n, index = 0, act_size = START_SIZE;
	int count_freed = 0, trash_var = 0, trash_var2 = 0;

	v = calloc(act_size, sizeof(int **));
	DIE(!v, CALLOC_ERROR);
	m = calloc(act_size, sizeof(int));
	DIE(!m, CALLOC_ERROR);
	n = calloc(act_size, sizeof(int));
	DIE(!n, CALLOC_ERROR);

	// do...while - menu
	do {
		fscanf(stdin, "%c", &command);
		switch (command) {
		case 'L':
			change_env(&m, &n, &v, &act_size, index, &count_freed);
			load_matrix(trash_var, trash_var2, m, n, v, &index);
			break;
		case 'D':
			print_dimensions(trash_var, m, n, index);
			break;
		case 'P':
			print_matrix(trash_var, m, n, index, v);
			break;
		case 'C':
			resize(trash_var, m, n, index, v);
			break;
		case 'M':
			change_env(&m, &n, &v, &act_size, index, &count_freed);
			mat_product(trash_var, trash_var2, m, n, &index, v);
			break;
		case 'O':
			sort_matrices(index, m, n, v);
			break;
		case 'T':
			transpose(trash_var, index, m, n, v);
			break;
		case 'R':
			log2_power_main(trash_var, trash_var2, index, m, n, v);
			break;
		case 'F':
			delete_element(trash_var, m, n, v, &index, &count_freed);
			change_env(&m, &n, &v, &act_size, index, &count_freed);
			break;
		case 'Q':
			clear_memory(m, n, v, index);
			break;
		default:
			if (command != '\n' && command != ' ')
				fprintf(stdout, "Unrecognized command\n");
		}
	} while (command != 'Q');
	return 0;
}
