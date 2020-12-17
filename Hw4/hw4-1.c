/* ID: 2015117609
 * NAME: Jihan Choi
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 //reference from data structure easily written in c language

#include <stdio.h>
#include <stdlib.h>	
#include <string.h>	
#define MEASURE_TIME	
#define MEASURE_MEMORY	
#ifdef MEASURE_TIME
#include <time.h>
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
};

struct WORDNODE *create_wnode(const char w[]) {
	struct WORDNODE *newnode;
	newnode = (struct WORDNODE*)malloc_c(sizeof(struct WORDNODE));
	newnode->word = strdup_c(w);
	newnode->count = 1;
	newnode->next = NULL;
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
	if (htab) {
		for (i = 0; i < htab->size; i++)
			free_wnodelist_recursive(htab->wnode[i]);
		free(htab);
	}
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
	char buffer[256];
	char *infile, *outfile;
	FILE *ifp, *ofp;
	int j;

#ifdef MEASURE_TIME
	clock_t start, end;
	double cpu_time_used;
	start = clock();
#endif

	if (argc != 4) {
		fprintf(stderr, "usage: %s hashtablesize infile outfile\n", argv[0]);
		exit(0);
	}
	else {
		htabsize = atoi(argv[1]);
		infile = argv[2];
		outfile = argv[3];
	}
	hashtable = create_word_hashtable(htabsize);
	if ((ifp = fopen(infile, "r")) == NULL) {
		fprintf(stderr, "cannot open file %s\n", infile);
		return -1;
	}
	else {
		for (j = 0; !feof(ifp); j++) {
			int flag = 1;
			int temp = 0;
			if (fscanf(ifp, "%s", buffer) != 1) break;
			else {
				int hash_value = hash_function(buffer, htabsize);
				struct WORDNODE* node_before = NULL;
				struct WORDNODE* node = hashtable->wnode[hash_value];
				for (; node; node_before = node, node = node->next) {
					temp = strcmp(buffer, node->word);
					if (temp < 0)
						break;
					else if (temp > 0)
						continue;
					else {
						node->count++;
						flag = 0;
						break;
					}
				}
				if (flag == 1) {
					struct WORDNODE* fresh = create_wnode(buffer);
					if (!node_before) {
						fresh->next = hashtable->wnode[hash_value];
						hashtable->wnode[hash_value] = fresh;
					}
					else {
						fresh->next = node_before->next;
						node_before->next = fresh;
					}
					hashtable->num_words++;
				}
			}
		}
	}
	fclose(ifp);

	if ((ofp = fopen(outfile, "w")) == NULL) {
		fprintf(stderr, "cannot open file for write %s\n", outfile);
		return -1;
	}
	else {
		fprintf(ofp, "hashtable %d buckets %d words\n", hashtable->size, hashtable->num_words);
		struct WORDNODE* ptr = NULL;
		for (int i = 0; i < htabsize; i++) {
			fprintf(ofp, "bucket %d", i);
			for (ptr = hashtable->wnode[i]; ptr; ptr = ptr->next)
				fprintf(ofp, " (%s %d)", ptr->word, ptr->count);
			fprintf(ofp, "\n");
		}
		fprintf(ofp, "\nalphabet-sorted %d words\n", hashtable->num_words);
		
		int count_temp=0, min_index = 0;
		struct WORDNODE** arr = (struct WORDNODE**)malloc_c(sizeof(struct WORDNODE*)*htabsize);
		for (int i = 0; i < htabsize; i++)
			arr[i] = hashtable->wnode[i];
		for (int i = 0; i < htabsize; i++)
			if (arr[i] != NULL) {
				ptr = arr[i];
				break;
			}

		while (count_temp < hashtable->num_words) {
			for (int i = 0; i < htabsize; i++) 
				if (arr[i] != NULL) 
					if (strcmp(arr[i]->word, ptr->word) < 0) {
						ptr = arr[i];
						min_index = i;
					}
			fprintf(ofp, "(%s %d) ", arr[min_index]->word, arr[min_index]->count);
			count_temp++;
			arr[min_index] = arr[min_index]->next;
			if (arr[min_index] != NULL)
				ptr = arr[min_index];
			else
				for (int i = 0; i < htabsize; i++)
					if (arr[i] != NULL) {
						ptr = arr[i];
						min_index = i;
						break;
					}
		}

		fclose(ofp);

#ifdef MEASURE_TIME
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		fprintf(stdout, "%.5f seconds\n", cpu_time_used);
#endif
#ifdef MEASURE_MEMORY
		fprintf(stdout, "%lu bytes ( %.3f x %d )\n",
			used_memory, (double)used_memory / (double)hashtable->num_words, hashtable->num_words);
#endif
		free(arr);
		free_hashtable(hashtable);
		return 0;
	}
}

