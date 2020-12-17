/*  ID: 2015117609
	Name: Jihan Choi
	OS: linux,Ubuntu 16.04
	Compiler version: gcc 5.4.0 20160609
*/

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

struct student { int id; char name[128], major[128]; };

int main(void) {
	struct student* myself;
	myself = (struct student*)malloc(sizeof(struct student));
	myself->id = 2015117609;
	strcpy(myself->name, "Jihan Choi");
	strcpy(myself->major, "Electronics Engineering");

	printf("ID: %d\n", myself->id);
	printf("NAME: %s\n", myself->name);
	printf("MAJOR: %s\n", myself->major);
	free(myself);
	return 0;
}