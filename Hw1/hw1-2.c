/*  ID: 2015117609
	Name: Jihan Choi
	OS: linux,Ubuntu 16.04
	Compiler version: gcc 5.4.0 20160609
*/

#include <stdio.h>
#include <stdlib.h>
#define SWAP(x,y,t) ( (t) = (x), (x) = (y), (y) = (t) )
#define COMPARE(x,y) ( (x) >= (y) ? 1 : 0 )

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("wrong input!!\n");
		return 0;
	}
	int arr[3], temp;
	arr[0] = atoi(argv[1]);
	arr[1] = atoi(argv[2]);
	arr[2] = atoi(argv[3]);

	if (COMPARE(arr[0], arr[1])) SWAP(arr[0], arr[1], temp);
	if (COMPARE(arr[1], arr[2])) SWAP(arr[1], arr[2], temp);
	if (COMPARE(arr[0], arr[1])) SWAP(arr[0], arr[1], temp);

	printf("%d %d %d\n", arr[0], arr[1], arr[2]);

	return 0;
}