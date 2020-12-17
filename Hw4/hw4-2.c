/* ID: 2015117609
 * NAME: Jihan Choi
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
*/
//reference from data structure easily written in c language

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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

struct WORDNODE {
	char *word;
	int count;
	struct WORDNODE *next;
	struct WORDNODE *prev;
	int code;
};

struct WORDNODE *create_wnode(const char w[]) {
	struct WORDNODE *newnode;
	newnode = (struct WORDNODE*)malloc_c(sizeof(struct WORDNODE));
	newnode->word = strdup_c(w);
	newnode->count = 1;
	newnode->next = NULL;
	newnode->prev = NULL;
	newnode->code = -1;
	return newnode;

}

void free_wnodelist_recursive(struct WORDNODE *node) {
	if (node) {
		free_wnodelist_recursive(node->next);
		free(node->word);
		free(node);
	}
}
struct WORDHASHTABLE {
	struct WORDNODE **wnode;
	int size;
	int num_words;
};
struct WORDHASHTABLE *create_word_hashtable(int hashtabsize)
{
	int i;
	struct WORDHASHTABLE *htab;
	htab = (struct WORDHASHTABLE*)malloc_c(sizeof(struct WORDHASHTABLE));
	htab->wnode = (struct WORDNODE**)malloc_c(sizeof(struct WORDNODE*)*hashtabsize);
	for (i = 0; i < hashtabsize; i++) htab->wnode[i] = NULL;
	htab->size = hashtabsize;
	htab->num_words = 0;
	return htab;
}
void free_hashtable(struct WORDHASHTABLE *htab) {

	int i;

	for (i = 0; i < htab->size; i++)

		free_wnodelist_recursive(htab->wnode[i]);

	free(htab);

}
int transform(char* key) {

	int number = 0;

	while (*key)

		number += *key++;

	return number;

}

int hash_function(char* key, int htabsize) {

	return transform(key) % htabsize;

}

int main(int argc, char *argv[])

{
	struct WORDHASHTABLE *hashtable;
	int htabsize;
	char buffer[256];	// input string
	char *infile, *outfile1, *outfile2;
	FILE *ifp, *ofp;
	int j;
	int max_index = 0, temp = 0, code = 0;
#ifdef MEASURE_TIME
	clock_t start, end;
	double cpu_time_used;
	start = clock();
#endif
	if (argc != 5) {
		fprintf(stderr, "usage: %s hashtablesize infile outfile1 outfile2\n", argv[0]);
		exit(0);
	}
	else {
		htabsize = atoi(argv[1]);
		infile = argv[2];
		outfile1 = argv[3];
		outfile2 = argv[4];
	}
	hashtable = create_word_hashtable(htabsize);
	if ((ifp = fopen(infile, "r")) == NULL) {
		fprintf(stderr, "cannot open file %s\n", infile);
		return -1;
	}
	else {
		for (j = 0; !feof(ifp); j++) {
			int flag = 1;
			int flag2 = 1;
		    temp = 0;
			if (fscanf(ifp, "%s", buffer) != 1) break;
			else {
				int hash_value = hash_function(buffer, htabsize);
				struct WORDNODE* node_before = NULL;
				struct WORDNODE* node = hashtable->wnode[hash_value];
				for (; node; node_before = node, node = node->next) {
					if (!strcmp(buffer, node->word)) {
						node->count++;
						flag = 0;
						break;
					}
				}
				if (flag) { 
					hashtable->num_words++;
					struct WORDNODE* fresh = create_wnode(buffer); 
					if (!node_before)
						hashtable->wnode[hash_value] = fresh;
					else {
						node = node_before; 
						if (node->count != 1) {
							node->next = fresh;
							fresh->prev = node;
						}
						else {
							for (; node != hashtable->wnode[hash_value] && node->count == 1; node = node->prev) { 
								temp = strcmp(buffer, node->word);
								if (temp < 0) 
									continue;
								else {
									fresh->prev = node;
									fresh->next = node->next;
									if (node->next != NULL)
										node->next->prev = fresh;
									node->next = fresh;
									flag2 = 0;
									break;
								}
							}
							if (flag2) {							
								if (node != hashtable->wnode[hash_value]) {
									fresh->prev = node;
									fresh->next = node->next;
									if (node->next != NULL)
										node->next->prev = fresh;
									node->next = fresh;
								}
								else {
									if (node->count != 1) {
										fresh->next = node->next;
										fresh->prev = node;
										if (node->next)
											node->next->prev = fresh;
										node->next = fresh;
									}
									else {
										if (strcmp(node->word, buffer) > 0) {
											fresh->next = hashtable->wnode[hash_value];
											node->prev = fresh;
											hashtable->wnode[hash_value] = fresh;
										}
										else {
											if (node->next)
												node->next->prev = fresh;
											fresh->next = node->next;
											fresh->prev = node;
											node->next = fresh;
										}
									}
								}
							}
						}
					}
				}
				else if (!flag) { 
					if (node->prev) {
						struct WORDNODE* ptr = NULL;
						if (node->prev->count <= node->count) {
							for (ptr = node->prev; ptr; ptr = ptr->prev) {
								if (node->count > ptr->count) {
									if(ptr != hashtable->wnode[hash_value])
										continue;
									else {
										node->prev->next = node->next;
										if (node->next)
											node->next->prev = node->prev;
										node->next = hashtable->wnode[hash_value];
										hashtable->wnode[hash_value]->prev = node;
										hashtable->wnode[hash_value] = node;
										node->prev = NULL;
										break;
									}
								}
								else if(node->count == ptr->count){
									temp = strcmp(buffer, ptr->word);
									if (temp < 0) {
										if(ptr != hashtable->wnode[hash_value])
											continue;
										else {
											node->prev->next = node->next;
											if (node->next)
												node->next->prev = node->prev;
											node->next = hashtable->wnode[hash_value];
											hashtable->wnode[hash_value]->prev = node;
											hashtable->wnode[hash_value] = node;
											node->prev = NULL;
											break;
										}
									}
									else {
										if (ptr->next != node) {																					
											node->prev->next = node->next;
											if (node->next)
												node->next->prev = node->prev;
											node->next = ptr->next;
											ptr->next->prev = node;
											node->prev = ptr;
											ptr->next = node;									
										}
										break;
									}
								}
								else {
									node->prev->next = node->next;
									if (node->next)
										node->next->prev = node->prev;
									node->next = ptr->next;
									node->prev = ptr;
									if (ptr->next)
										ptr->next->prev = node;
									ptr->next = node;
									break;
								}
							}
						}
					}
				}
			}
		}
		rewind(ifp);
	}

	if ((ofp = fopen(outfile1, "w")) == NULL) {
		fprintf(stderr, "cannot open file for write %s\n", outfile1);
		return -1;
	}
	else {
		fprintf(ofp,"hashtable %d buckets %d words\n", hashtable->size, hashtable->num_words);
		struct WORDNODE* ptr = NULL;
		for (int i = 0; i < htabsize; i++) {
			fprintf(ofp,"bucket %d", i);
			for (ptr = hashtable->wnode[i]; ptr; ptr = ptr->next)
				fprintf(ofp," (%s %d %d)", ptr->word, ptr->count, ptr->code);
			fprintf(ofp,"\n");
		}

		fprintf(ofp,"\ncount-alphabet-sorted %d words\n", hashtable->num_words);
		struct WORDNODE** arr = (struct WORDNODE**)malloc_c(sizeof(struct WORDNODE*)*htabsize);
		for (int i = 0; i < htabsize; i++)
			arr[i] = hashtable->wnode[i];
		for (int i = 0; i < htabsize; i++)
			if (arr[i] != NULL) {
				ptr = arr[i];
				max_index = i;
				break;
			}
		while (code < hashtable->num_words) {
			for (int i = 0; i < htabsize; i++) {
				if (arr[i] != NULL) {
					if (ptr->count < arr[i]->count) {
						ptr = arr[i];
						max_index = i;
					}
					else if (ptr->count == arr[i]->count) {
						temp = strcmp(arr[i]->word, ptr->word);
						if (temp < 0) {
							ptr = arr[i];
							max_index = i;
						}
					}
				}
			}
			arr[max_index]->code = code++;
			fprintf(ofp,"(%s %d %d) ", arr[max_index]->word, arr[max_index]->count, arr[max_index]->code);
			arr[max_index] = arr[max_index]->next;
			if (arr[max_index] != NULL)
				ptr = arr[max_index];
			else
				for (int i = 0; i < htabsize; i++)
					if (arr[i] != NULL) {
						ptr = arr[i];
						max_index = i;
						break;
					}
		}
		fprintf(ofp,"\n\nhashtable %d buckets %d words\n", hashtable->size, hashtable->num_words);
		for (int i = 0; i < htabsize; i++) {
			fprintf(ofp,"bucket %d", i);
			for (ptr = hashtable->wnode[i]; ptr; ptr = ptr->next)
				fprintf(ofp," (%s %d %d)", ptr->word, ptr->count, ptr->code);
			fprintf(ofp,"\n");
		}
		fprintf(ofp,"\n");
		fclose(ofp);
		free(arr);
	}
	
	if ((ofp = fopen(outfile2, "w")) == NULL) {
		fprintf(stderr, "cannot open file for write %s\n", outfile2);
		return -1;
	}
	else {
		for (j = 0; !feof(ifp); j++) {
			if (fscanf(ifp, "%s", buffer) != 1) break;
			else {
				int hash_value = hash_function(buffer, htabsize);
				struct WORDNODE* node = hashtable->wnode[hash_value];
				for (; node; node = node->next)
					if (!strcmp(buffer, node->word)) {
						code = node->code;
						break;
					}
			}
			fprintf(ofp,"%d ", code);
		}
		fclose(ofp);
	}
	fclose(ifp);

#ifdef MEASURE_TIME
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	fprintf(stdout, "%.5f seconds\n", cpu_time_used);
#endif
#ifdef MEASURE_MEMORY
	fprintf(stdout, "%lu bytes ( %.3f x %d )\n",
		used_memory, (double)used_memory / (double)hashtable->num_words, hashtable->num_words);
#endif
	free_hashtable(hashtable);
	return 0;

}