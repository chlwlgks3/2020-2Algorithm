/* ID: 2015117609
 * NAME: Jihan Choi
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 //reference from brain stimulating algorithm

#include<stdio.h>
#include<stdlib.h>   
#include<string.h>   
#include<stdint.h>
#define MEASURE_TIME   
#define MEASURE_MEMORY 
#ifdef MEASURE_TIME
#include<time.h>
#endif

#ifdef MEASURE_MEMORY
static size_t used_memory = 0;
void *malloc_c(size_t size)
{
	if (size > 0) {
		used_memory += size;
		return malloc(size);
	}
	else return NULL;
}

char *strdup_c(const char *s) {
	int size;
	size = strlen(s) + 1; 
	used_memory += size;
	return strdup(s);
}

#else
void *malloc_c(size_t size) { return malloc(size); }
char *strdup_c(const char *s) { return strdup(s); }
#endif
typedef struct structLCSTable {
	int** Data;
}LCSTable;
LCSTable Table;
char* temp;

int LCS(char* X, char* Y, int i, int j, LCSTable* Table) {
	int m = 0;
	int n = 0;
	for (m = 0; m <= i; m++)
		Table->Data[m][0] = 0;
	for (n = 0; n <= j; n++)
		Table->Data[0][n] = 0;
	for (m = 1; m <= i; m++) {
		for (n = 1; n <= j; n++) {
			if (X[m - 1] == Y[n - 1])
				Table->Data[m][n] = Table->Data[m - 1][n - 1] + 1;
			else {
				if (Table->Data[m][n - 1] >= Table->Data[m - 1][n])
					Table->Data[m][n] = Table->Data[m][n - 1];
				else
					Table->Data[m][n] = Table->Data[m - 1][n];
			}
		}
	}
	return Table->Data[i][j];
}

void findLCS(char* LCS, char* X, char* Y, int m, int n, LCSTable* Table)
{
	if (m == 0 || n == 0)
		return;
	if (Table->Data[m][n] > Table->Data[m][n - 1] && Table->Data[m][n] > Table->Data[m - 1][n] && Table->Data[m][n] > Table->Data[m - 1][n - 1]) {
		strcpy(temp, LCS);
		sprintf(LCS, "%c%s", X[m - 1], temp);
		findLCS(LCS, X, Y, m - 1, n - 1, Table);
	}
	else if (Table->Data[m][n] > Table->Data[m - 1][n] && Table->Data[m][n] == Table->Data[m][n - 1])
		findLCS(LCS, X, Y, m, n - 1, Table);
	else
		findLCS(LCS, X, Y, m - 1, n, Table);
}

void cirshift(char* s1, int size, int shamt) {
	if (shamt == 0) {
		strcpy(temp, s1);
		return;
	}
	for (int i = 0; i < size; i++) {
		temp[shamt++] = s1[i];
		if (shamt == size)
			shamt = 0;
	}
	temp[size] = '\0';
}

int findLCS_circ(char* s1, char* s2,int origin)
{
	int* arr = (int*)malloc_c(sizeof(int)*(strlen(s1) - 1));
	int max = 0;
	int max_shamt = 0;
	for (unsigned int i = 1; i < strlen(s1); i++) {
		cirshift(s1, strlen(s1), i);
		arr[i - 1] = LCS(temp, s2, strlen(temp), strlen(s2), &Table);
	}
	for (unsigned int i = 0; i < strlen(s1) - 1; i++) {
		if (arr[i] > max) {
			max = arr[i];
			max_shamt = i + 1;
		}
	}
	free(arr);
	if (max > origin)
		return max_shamt;
	else
		return 0;
}

int main(int argc, char *argv[])
{
	char *s1, *s2;
	int l1, l2, min_l;
	char *lcs; 
	int cut_max; 
	int i;
#ifdef MEASURE_TIME
	clock_t start, end;
	double cpu_time_used;
#endif
	if (argc != 3) {
		fprintf(stderr, "usage: %s string1 string2\n", argv[0]);
		exit(0);
	}
	else {
		fprintf(stdout, "$ ");
		for (i = 0; i < argc; i++) fprintf(stdout, "%s ", argv[i]);
		fprintf(stdout, "\n");
	}

	s1 = strdup_c(argv[1]);
	s2 = strdup_c(argv[2]);

#ifdef MEASURE_TIME
	start = clock();
#endif
	l1 = strlen(s1);
	l2 = strlen(s2);
	min_l = (l1 < l2) ? l1 : l2; 
	Table.Data = (int**)malloc_c(sizeof(int*)*(l1 + 1));
	for (i = 0; i < l1 + 1; i++) 
		Table.Data[i] = (int*)malloc_c(sizeof(int)*(l2 + 1));
	for (i = 0; i < l1 + 1; i++) 
		for (int j = 0; j < l2 + 1; j++)
			Table.Data[i][j] = 0;

	int origin = LCS(s1, s2, l1, l2, &Table);
	lcs = (char*)malloc_c(Table.Data[l1][l2] + 1);
	temp = (char*)malloc_c(min_l+1);
	findLCS(lcs, s1, s2, l1, l2, &Table);
	lcs[Table.Data[l1][l2]] = '\0';
	fprintf(stdout, "1) S1 S2 LCS = %s %s %s\n", s1, s2, lcs);

	cut_max = findLCS_circ(s1, s2,origin);
	if (cut_max) {
		cirshift(s1, strlen(s1), cut_max);
		LCS(temp, s2, strlen(temp), strlen(s2), &Table);
		strcpy(s1, temp);
		findLCS(lcs, s1, s2, strlen(temp), strlen(s2), &Table);
		lcs[Table.Data[l1][l2]] = '\0';
	}
	fprintf(stdout, "2) S1(shift=%d) S2 LCS = %s %s %s\n",
		cut_max, s1, s2, lcs);

#ifdef MEASURE_TIME
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	fprintf(stdout, "%.5f seconds\n", cpu_time_used);
#endif
#ifdef MEASURE_MEMORY
	fprintf(stdout, "%lu bytes ( %.3f x %d )\n",
		used_memory, (double)used_memory / (double)(l1*l2), (l1*l2));
#endif
	fprintf(stdout, "\n");
	for(int i = 0;i<l1+1;i++)
		free(Table.Data[i]);
	free(Table.Data);
	free(lcs);
	free(temp);
	free(s1);
	free(s2);
	
	return 0;
}
