/* ID: 2015117609	
 * NAME: Jihan Choi	
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 //reference from Fundamentals of Data Structures in C
#include<stdio.h>
#include<stdlib.h>	
#include<string.h>
#define MAX_NUM_STR	4096
#define MAX_STR_LEN	256
#define SWAP(x,y,t) ( strcpy((t) ,(x)),strcpy((x) ,(y)),strcpy((y) ,(t)) )

void print_chararr_2dim(FILE *fp, char A[][MAX_STR_LEN], int N)
{
	int i;
	fprintf(fp, "%d\n", N);
	for (i = 0; i < N; i++) fprintf(fp, "%s ", A[i]);
	fprintf(fp, "\n");
}

void read_chararr_2dim_textfile(const char infile[],
	char A[][MAX_STR_LEN], int *pN)
{
	int i;
	FILE *fp;

	if (infile == NULL) {
		fprintf(stderr, "NULL file name\n");
		return;
	}

	fp = fopen(infile, "r");
	if (!fp) {
		fprintf(stderr, "cannot open file %s\n", infile);
		return;
	}
	else {
		if (fscanf(fp, "%d", pN) != 1 || *pN <= 0) {
			fprintf(stderr, "cannot read number of elements %s\n", infile);
			return;
		}
		else {
			if (*pN > MAX_NUM_STR) {
				fprintf(stderr, "cannot read %d words (%d > %d)\n", *pN, *pN, MAX_NUM_STR);
				return;
			}
			for (i = 0; i < (*pN); i++) {
				if (fscanf(fp, "%s", A[i]) != 1) {
					fprintf(stderr, "cannot read value at %d/%d\n", i + 1, (*pN));
					*pN = i;
					return;
				}
			}
		}
		fclose(fp);
	}
}

void write_chararr_2dim_textfile(const char outfile[],
	char A[][MAX_STR_LEN], int N)
{
	FILE *fp;

	if (outfile == NULL) {
		fprintf(stderr, "NULL file name\n");
		return;
	}
	fp = fopen(outfile, "w");
	if (!fp) {
		fprintf(stderr, "cannot open file for write %s\n", outfile);
	}
	else {
		print_chararr_2dim(fp, A, N);
		fclose(fp);
	}
}

void insertion_sort_chararr_2dim(char arr[][MAX_STR_LEN], int n) {
	int i, j;
	char key[MAX_STR_LEN];
	for (i = 1; i < n; i++) {
		strcpy(key, arr[i]);
		for (j = i - 1; j >= 0 && strcmp(arr[j], key) >= 0; j--)
			strcpy(arr[j + 1], arr[j]);
		strcpy(arr[j + 1], key);
	}
}

void selection_sort_chararr_2dim(char arr[][MAX_STR_LEN], const int n) {
	int i, j, least;
	char temp[MAX_STR_LEN];
	for (i = 0; i < n - 1; i++) {
		least = i;
		for (j = i + 1; j < n; j++)
			if (strcmp(arr[j], arr[least]) == -1) least = j;
		if (i != least)
			SWAP(arr[i], arr[least], temp);
	}
}

void bubble_sort_chararr_2dim(char arr[][MAX_STR_LEN], int n) {
	int i, j, check = 1;
	char temp[MAX_STR_LEN];
	for (i = n - 1; i > 0 && check == 1; i--) {
		check = 0;
		for (j = 0; j < i; j++)
			if (strcmp(arr[j], arr[j + 1]) > 0) {
				check = 1;
				SWAP(arr[j], arr[j + 1], temp);
			}
	}
}

int main(int argc, char *argv[])
{
	int numElements;
	char A[MAX_NUM_STR][MAX_STR_LEN];
	int method;

	if (argc != 4) {
		fprintf(stderr, "%d\n", argc);
		fprintf(stderr, "usage: %s method infile outfile\n", argv[0]);
		fprintf(stderr, " method 1: insertion, 2: selection, 3: bubble\n");
		exit(0);
	}

	method = atoi(argv[1]);

	read_chararr_2dim_textfile(argv[2], A, &numElements);

	if (numElements > 0 && numElements <= MAX_NUM_STR) {
		switch (method) {
		case 1:
			insertion_sort_chararr_2dim(A, numElements);
			break;
		case 2:
			selection_sort_chararr_2dim(A, numElements);
			break;
		case 3:
			bubble_sort_chararr_2dim(A, numElements);
			break;
		default:
			break;
		}
		write_chararr_2dim_textfile(argv[3], A, numElements);
	}
}