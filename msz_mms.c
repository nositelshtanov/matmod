#include <stdlib.h>
#include <stdio.h>

// initial allocation of the transport problem
// by the northwest corner method and minimum cost method

void msz(int *a_vector, int a_len, int *b_vector, int b_len, int **cost_matrix);
void mms(int *a_vector, int a_len, int *b_vector, int b_len, int **cost_matrix);

struct edge {
	int ai;
	int bi;
	int cost;
};

void insertion_sort(struct edge *arr, int size);

int main() {
	int i, j;
	int a_len, b_len;
	int *a_vector, *b_vector;
	int **cost_matrix;
	int method = 0;

	printf("Enter A vector length: ");
	scanf("%d", &a_len);

	printf("Enter B vector length: ");
	scanf("%d", &b_len);
	
	printf("Enter A vector: ");

	i = 0;
	a_vector = malloc(sizeof(int)*a_len);

	while (i < a_len) {
		scanf("%d", a_vector + i);
		i++;
	}

	printf("Enter B vector: ");

	i = 0;
	b_vector = malloc(sizeof(int)*b_len);

	while (i < b_len) {
		scanf("%d", b_vector + i);
		i++;
	}

	printf("Enter C matrix: \n");
	
	cost_matrix = malloc(sizeof(int*)*a_len);
	
	for (i = 0; i < a_len; i++) {
		cost_matrix[i] = malloc(sizeof(int)*b_len);

		for (j = 0; j < b_len; j++) {
			scanf("%d", &cost_matrix[i][j]);
		}
	}

	printf("Enter method (0 - msz, 1 - mms): ");
	scanf("%d", &method);
	
	if (method == 0) {
		msz(a_vector, a_len, b_vector, b_len, cost_matrix);
	} else if (method == 1) {
		mms(a_vector, a_len, b_vector, b_len, cost_matrix);
	} else {
		printf("invalid method number\n");
	}
	
	free(a_vector);
	free(b_vector);
	free(cost_matrix);

	return 0;
}

void insertion_sort(struct edge *arr, int size)
{
	int i, j;
	struct edge a;

	for (i = 0; i < size; i++) {
		a = arr[i];
		j = i;
	
		while (arr[j-1].cost > a.cost && j > 0) {
			arr[j] = arr[j-1];
			j--;	
		}
		
		arr[j] = a;	
	} 	
}

void mms(int *a_vector, int a_len, int *b_vector, int b_len, int **cost_matrix) {
	int i, j, c;
	int a_summ = 0, b_summ = 0, summary_cost = 0;

	int (*matrix)[b_len] = malloc(sizeof(int)*a_len*b_len);

	struct edge *edges = malloc(sizeof(struct edge)*a_len*b_len);

	int *a_tmp = malloc(sizeof(int)*a_len);
	int *b_tmp = malloc(sizeof(int)*b_len);

	for (i = 0; i < a_len; i++) {
		a_summ += a_vector[i];
		a_tmp[i] = a_vector[i];
	}

	for (i = 0; i < b_len; i++) {
		b_summ += b_vector[i];
		b_tmp[i] = b_vector[i];
	}
	
	if (a_summ != b_summ) {
		printf("Not balanced");
		goto clean;
	}
	
	for (i = 0, c = 0; i < a_len; i++) {
		for (j = 0; j < b_len; j++, c++) {
			matrix[i][j] = 0;
			edges[c].ai = i;
			edges[c].bi = j;
			edges[c].cost = cost_matrix[i][j];
		}
	}

	insertion_sort(edges, a_len*b_len);

	for (c = 0; c < a_len*b_len; c++) {
		struct edge e = edges[c];
	
		i = e.ai;
		j = e.bi;	

		if (a_tmp[i] == 0 || b_tmp[j] == 0) continue;
		
		if (a_tmp[i] == b_tmp[j]) {
			summary_cost += e.cost * a_tmp[i];
			matrix[i][j] = a_tmp[i];
			a_tmp[i] = b_tmp[j] = 0;
		} else if (a_tmp[i] > b_tmp[j]) {
			summary_cost += e.cost * b_tmp[j];
			matrix[i][j] = b_tmp[j];
			a_tmp[i] -= b_tmp[j];
			b_tmp[j] = 0;
		} else {
			summary_cost += e.cost * a_tmp[i];
			matrix[i][j] = a_tmp[i];
			b_tmp[j] -= a_tmp[i];
			a_tmp[i] = 0;
		}
	}
	
	printf("Result: \n");

	for (i = 0; i < a_len; i++) {
		for (j = 0; j < b_len; j++) {
			printf("%d ", matrix[i][j]);
		}

		printf("\n");
	}

	printf("F = %d\n", summary_cost);

clean: 	free(a_tmp);
	free(b_tmp);
	free(matrix);
	free(edges);
}

void msz(int *a_vector, int a_len, int *b_vector, int b_len, int **cost_matrix) {
	int i, j;
	int cur_a, cur_b;
	int summary_cost = 0;

	int (*matrix)[b_len] = malloc(sizeof(int)*a_len*b_len);

	int a_summ = 0, b_summ = 0;

	for (i = 0; i < a_len; i++) {
		a_summ += a_vector[i];
	}

	for (i = 0; i < b_len; i++) {
		b_summ += b_vector[i];
	}
	
	if (a_summ != b_summ) {
		printf("Not balanced");
		goto clean;
	}
	
	for (i = 0; i < a_len; i++) {
		for (j = 0; j < b_len; j++) {
			matrix[i][j] = 0;
		}
	}

	i = j = 0;
	cur_a = a_vector[i];
	cur_b = b_vector[j];

	while (i < a_len && j < b_len) {
		if (cur_a == cur_b) {
			matrix[i][j] = cur_b;
			summary_cost += cur_b * cost_matrix[i][j];
			i++;
			j++;
			cur_a = a_vector[i];
			cur_b = b_vector[j];
		} else if (cur_a > cur_b) {
			matrix[i][j] = cur_b;
			summary_cost += cur_b * cost_matrix[i][j];
			cur_a = cur_a - cur_b;
			j++;
			cur_b = b_vector[j];
		} else {
			matrix[i][j] = cur_a;
			summary_cost += cur_a * cost_matrix[i][j];
			cur_b = cur_b - cur_a;
			i++;
			cur_a = a_vector[i];
		}
	}
		
	printf("Result: \n");

	for (i = 0; i < a_len; i++) {
		for (j = 0; j < b_len; j++) {
			printf("%d ", matrix[i][j]);
		}

		printf("\n");
	}

	printf("F = %d\n", summary_cost);

clean: 	free(matrix);
}
