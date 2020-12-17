/* ID: 2015117609
 * NAME: Jihan Choi
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 //reference from data structure easily written in C language

#include<stdio.h>
#include<stdlib.h>   
#include<string.h>   
#include<stdint.h>
#define SWAP(x,y,t) ( strcpy(t,x),strcpy(x,y),strcpy(y,t) )
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
#else
void *malloc_c(size_t size) { return malloc(size); }
#endif

char** sorted = NULL;
char* pivot = NULL;
char* temp = NULL;
char* read_bytes(const char infile[], long *pNbytes)
{
	char *B;
	long n;
	FILE *fp;

	if (infile == NULL) {
		fprintf(stderr, "NULL input file name\n");
		return NULL;
	}

	fp = fopen(infile, "r");
	if (!fp) {
		fprintf(stderr, "cannot open file %s for read\n", infile);
		return NULL;
	}
	else {
		fseek(fp, 0, SEEK_END);   
		*pNbytes = ftell(fp);  
		if (*pNbytes <= 0) return NULL;   
		rewind(fp);
		B = (char*)malloc_c(*pNbytes + 1);
		n = fread(B, 1, *pNbytes, fp);
		if (n != *pNbytes) {
			fprintf(stderr, "file %s, %ld/%ld bytes read\n", infile, n, *pNbytes);
		}
		B[*pNbytes] = '\0';
		fclose(fp);
		return B;
	}
}

void write_bytes(const char outfile[], const char B[], long Nbytes)
{
	FILE *fp;
	long n;
	if (outfile == NULL) {
		fprintf(stderr, "NULL output file name\n");
		return;
	}

	fp = fopen(outfile, "w");
	if (!fp) {
		fprintf(stderr, "cannot open file %s for write\n", outfile);
	}
	else {
		n = fwrite(B, 1, Nbytes, fp);
		if (n != Nbytes) {
			fprintf(stderr, "file %s, %ld/%ld bytes written\n", outfile, n, Nbytes);
		}
		fclose(fp);
	}
}

void merge(char** list, int left, int mid, int right) {
	int i, j, k, l;
	i = left; j = mid + 1; k = left;
	while (i <= mid && j <= right) {
		if (strcmp(list[i], list[j]) <= 0)
			strcpy(sorted[k++], list[i++]);
		else
			strcpy(sorted[k++], list[j++]);
	}
	if (i > mid)
		for (l = j; l <= right; l++)
			strcpy(sorted[k++], list[l]);
	else
		for (l = i; l <= mid; l++)
			strcpy(sorted[k++], list[l]);
	for (l = left; l <= right; l++)
		strcpy(list[l], sorted[l]);
}

void merge_sort(char** list, int left, int right) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		merge_sort(list, left, mid);
		merge_sort(list, mid + 1, right);
		merge(list, left, mid, right);
	}
}

int partition(char** list, int left, int right) {
	int low, high;
	low = left;
	high = right + 1;
	strcpy(pivot, list[left]);
	do {
		do
			low++;
		while (low <= right && strcmp(list[low], pivot)<0);
		do
			high--;
		while (high >= left && strcmp(list[high], pivot)>0);
		if (low < high) SWAP(list[low], list[high], temp);
	} while (low < high);
	SWAP(list[left], list[high], temp);
	return high;
}

void quick_sort(char** list, int left, int right) {
	if (left < right) {
		int q = partition(list, left, right);
		quick_sort(list, left, q - 1);
		quick_sort(list, q + 1, right);
	}
}

int main(int argc, char *argv[])
{
	long numBytes, arr_len;
	char *B;
	int method;
	int fixdotwordsize;
#ifdef MEASURE_TIME
	clock_t start, end;
	double cpu_time_used;
#endif

	if (argc != 5) {
		fprintf(stderr, "usage: %s method fixedwordsize infile outfile\n", argv[0]);
		fprintf(stderr, " method 1: mergesort, 2: quicksort\n");
		exit(0);
	}
	method = atoi(argv[1]);
	fixdotwordsize = atoi(argv[2]) + 1; 
	B = read_bytes(argv[3], &numBytes);
	arr_len = numBytes / fixdotwordsize;
	char** ptr_arr = (char**)malloc_c(arr_len * sizeof(char*));
	for (int i = 0; i < arr_len; i++)
		ptr_arr[i] = (char*)malloc_c(fixdotwordsize);
	int index_temp = 0;
	int j = 0;
	for (int i = 0; i < arr_len; i++) {
		while ((j + 1) % (fixdotwordsize) != 0 || j == 0) {
			ptr_arr[i][index_temp++] = B[j++];
		}
		ptr_arr[i][index_temp] = '\0';
		index_temp = 0; j++;
	}

#ifdef MEASURE_TIME
	start = clock();
#endif
	if (B != NULL && numBytes > 0) {
		switch (method) {
		case 1:
			sorted = (char**)malloc_c(arr_len * sizeof(char*));
			for (int i = 0; i < arr_len; i++)
				sorted[i] = (char*)malloc_c(fixdotwordsize);
			merge_sort(ptr_arr, 0, arr_len - 1);
			break;
		case 2:
			pivot = (char*)malloc_c(fixdotwordsize);
			temp = (char*)malloc_c(fixdotwordsize);
			quick_sort(ptr_arr, 0, arr_len - 1);
			break;
		default:
			break;
		}

#ifdef MEASURE_TIME
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		fprintf(stdout, "%.5f seconds\n", cpu_time_used);
#endif
#ifdef MEASURE_MEMORY
		fprintf(stdout, "%lu bytes ( %.3f x %ld )\n",
			used_memory, (double)used_memory / (double)numBytes, numBytes);

#endif

		for (int i = 0; i < arr_len; ++i)
		{
			strcpy(B + i * fixdotwordsize, ptr_arr[i]);
			*(B + (i + 1) * fixdotwordsize - 1) = '.';
		}
		write_bytes(argv[4], B, numBytes);
		free(B);
	}
	if (method == 1) {
		for (int q = 0; q < arr_len; q++)
			free(sorted[q]);
		free(sorted);
	}
	else {
		free(pivot);
		free(temp);
	}
	for (int i = 0; i < arr_len; i++)
		free(ptr_arr[i]);
	free(ptr_arr);

	return 0;
}