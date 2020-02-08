/*
    Multi-Value Map ADT: via Hash Table
    Collision strategy: Linear Probe
    The maximum number of attempt is 100
    Programme will stop when hitting the number and suggest to change a hash function
    Hash function used now is copied from lecture note
    All data will be inserted into a static array
    Time complexity of insertion, search and deletion can approximately reach O(1)
    A few possible collisions will affect time complexity 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPECIALCHARNUM 5
#define MAXLENGTH 200
#define MAGICHASH1 5381
#define MAGICHASH2 33
#define MAXATTEMPT 100
/* The number of items in cmudict.txt */
#define MAXITEM 134298
/* The size of hash table is  */
/* Next prime number after MAXITEM times 4 */
/* As being told in the lecture */
#define HTSIZE 537197

#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

struct node{
    char* value;
    struct node* next;
};
typedef struct node node;

struct ht{
    char* key;
    node* dataHead;
    int keyNum;
    int hashNum;
    int valueNum;
};
typedef struct ht ht;

struct mvm{
    ht* head;
    int numkeys;
};
typedef struct mvm mvm;

/* Calculate hash number of a given string */
unsigned long getHash(char* string);

/* Allocate valueList node */
node* allocateValue(char* string);

mvm* mvm_init(void);

/* Number of key/value pairs stored */
int mvm_size(mvm* m);

/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data);
void insert2empty(ht* curr, char* key, char* data);
void insert2same(ht* curr, char* data);
void linearProbe(mvm* m, char* key, char* data, int hashNum);

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m);
int partialLen(ht* curr);

/* Remove one key/value */
void mvm_delete(mvm* m, char* key);
void deleteAll(ht* curr);
void partialDel(ht* curr);
void secondDel(mvm* m, char* key, int hashNum);

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key);

/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n);

/* Free & set p to NULL */
void mvm_free(mvm** p);
