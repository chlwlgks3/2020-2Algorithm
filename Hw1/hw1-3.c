/*  ID: 2015117609
	Name: Jihan Choi
	OS: linux,Ubuntu 16.04
	Compiler version: gcc 5.4.0 20160609
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]) {
	if (argc != 2) {
		printf("wrong input!!\n");
		return 0;
	}
	FILE* stream = fopen(argv[1], "r");
	int* arr;
	int size,min=0,max=0,sum=0,sqr=0;
	int data;

	fscanf(stream, "%d", &size);
	arr = (int*)malloc(sizeof(*arr)*size);
	for (int i = 0; i < size; i++) {
		fscanf(stream, "%d", &data);
		arr[i] = data;
	}
	min = arr[0];
	for (int i = 0; i < size; i++) {
		if (arr[i] < min)
			min = arr[i];
		if (arr[i] > max)
			max = arr[i];
		sum += arr[i];
		sqr += arr[i] * arr[i];
	}
	double mean = (double)sum / (double)size;
	double variance = (double)sqr / (double)size - mean * mean;

	printf("#data\tmin\tmax\tmean\tvariance\n");
	printf("%d\t%d\t%d\t%.1f\t%.1f\n",size,min, max, mean, variance);
	free(arr);
	fclose(stream);
	return 0;
}