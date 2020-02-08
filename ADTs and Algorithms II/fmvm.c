#include "fmvm.h"

/* This hash calculation work is mostly copied from */
/* the data structure lecture note */
unsigned long getHash(char* string)
{
    int c = 0;
    unsigned long hash = MAGICHASH1;

    while ((c = (*string++))){
        hash = hash * MAGICHASH2 ^ c;
    }
    return hash % HTSIZE;
}

node* allocateValue(char* string)
{
    node* p;

    p = (node*)malloc(sizeof(node));
    p->value = (char*)calloc(MAXLENGTH, sizeof(char));

    if (p->value == NULL || p == NULL){
        ON_ERROR("Failed to allocate node.\n");
    }

    strcpy(p->value, string);
    p->next = NULL;

    return p;
}

mvm* mvm_init(void)
{
    static ht hashtable[HTSIZE];
    mvm* p;
    int i;

    for (i = 0; i < HTSIZE; ++i){
        hashtable[i].valueNum = hashtable[i].hashNum = 0;
        hashtable[i].keyNum = 0;
    }

    p = (mvm*)malloc(sizeof(mvm));
    if (p == NULL){
        ON_ERROR("Creation of hash table failed.\n");
    }
    p->numkeys = 0;
    p->head = hashtable;

    return p;
}

/* Number of key/value pairs stored */
int mvm_size(mvm* m)
{
    if (m == NULL){
        return 0;
    }
    return m->numkeys;
}

/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data)
{
    int hashNum;
    ht* curr;

    if (m == NULL || key == NULL || data == NULL){
        return;
    }

    m->numkeys++;
    hashNum = getHash(key);
    curr = &m->head[hashNum];
    curr->hashNum = hashNum;

    if (curr->keyNum == 0){
        insert2empty(curr, key, data);
        return;
    }
    if (strcmp(curr->key, key) == 0){
        insert2same(curr, data);
        return;
    }
    else{
        linearProbe(m, key, data, hashNum);
        return;
    }
    
}

void insert2empty(ht* curr, char* key, char* data)
{
    curr->keyNum++;
    curr->valueNum++;
    curr->key = (char*)calloc(strlen(key) + 1, sizeof(char));
    curr->dataHead = allocateValue(data);
    if (curr->key == NULL || curr->dataHead == NULL){
        ON_ERROR("Failed to insert key and data into empty hashtable cell.\n");
    }

    strcpy(curr->key, key);
}

void insert2same(ht* curr, char* data)
{
    int i;
    node* ptr;

    ptr = curr->dataHead;
    for (i = 0; i < curr->valueNum - 1; ++i){
        ptr = ptr->next;
    }
    ptr->next = allocateValue(data);
    curr->valueNum++;
}

void linearProbe(mvm* m, char* key, char* data, int hashNum)
{
    int i;
    ht* curr;

    for (i = 0; i < MAXATTEMPT; ++i){
        curr = &m->head[hashNum--];
        if (curr->keyNum == 0){
            insert2empty(curr, key, data);
            return;
        }
        if (strcmp(curr->key, key) == 0){
            insert2same(curr, data);
            return;
        }
    }
    ON_ERROR("Reached the maximum attempt number. Should change the hash function.\n");
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
    int* exist, i, j, cnt = 0, sum = 0, k = 0;
    ht* curr;
    node* temp;
    char* string;

    exist = (int*)calloc(m->numkeys, sizeof(int));

    for (i = 0; i < HTSIZE; ++i){
        curr = &m->head[i];
        if (curr->hashNum != 0){
            exist[cnt++] = i;
            sum += partialLen(curr);
        }
    }

    string = (char*)calloc(sum, sizeof(char));
    for (i = 0; i < m->numkeys; ++i){
        cnt = exist[i];
        curr = &m->head[cnt];
        temp = curr->dataHead;

        for (j = 0; j < curr->valueNum; ++j){
            k += sprintf(string + k, "[%s](%s) ", curr->key, temp->value);
            temp = temp->next;
        }
    }
    free(exist);
    return string;
}

int partialLen(ht* curr)
{
    int sum = 0, i;
    node* temp = curr->dataHead;

    sum += SPECIALCHARNUM * curr->valueNum;
    sum += (int)strlen(curr->key) + 1;
    for (i = 0; i < curr->valueNum; ++i){
        sum += (int)strlen(temp->value) + 1;
        temp = temp->next;
    }
    return sum;
}

void mvm_delete(mvm* m, char* key)
{
    int hashNum, i;
    ht* curr;

    if (m == NULL || key == NULL || strcmp(key, "") == 0){
        return;
    }
    m->numkeys--;
    hashNum = getHash(key);
    for (i = 0; i < MAXATTEMPT; ++i){
        curr = &m->head[hashNum];
        if (curr->valueNum == 1 && strcmp(curr->key, key) == 0){
            deleteAll(curr);
            return;
        }
        if (strcmp(curr->key, key) == 0){
            partialDel(curr);
            return;
        }
        else{
            secondDel(m, key, hashNum);
            return;
        }
    }
    
}

void deleteAll(ht* curr)
{
    curr->keyNum = curr->valueNum = 0;
    curr->hashNum = 0;
    free(curr->key);
    free(curr->dataHead->value);
    free(curr->dataHead);
}

void partialDel(ht* curr)
{
    int i;
    node* temp = curr->dataHead;

    curr->valueNum--;
    for (i = 0; i < curr->valueNum; ++i){
        temp = temp->next;
    }
    free(temp->value);
    free(temp);
}

void secondDel(mvm* m, char* key, int hashNum)
{
    int i;
    ht* curr;

    for (i = 0; i < MAXATTEMPT; ++i){
        curr = &m->head[hashNum--];
        if (curr->valueNum == 1 && strcmp(curr->key, key) == 0){
            deleteAll(curr);
            return;
        }
        if (strcmp(curr->key, key) == 0){
            partialDel(curr);
            return;
        }
    }
    ON_ERROR("Failed to find this key in the hash table.\n");
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
    int hashNum, i;
    ht* curr;

    hashNum = getHash(key);
    hashNum++;

    for (i = 0; i < MAXATTEMPT; ++i){
        curr = &m->head[hashNum--];
        if (curr->keyNum == 1 && strcmp(curr->key, key) == 0){
            return curr->dataHead->value;
        }
    }
    return NULL;
}

/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
    int hashNum, i, j;
    ht* curr;
    char** ptr;
    node* temp;

    hashNum = getHash(key);
    hashNum++;

    for (i = 0; i < MAXATTEMPT; ++i){
        curr = &m->head[hashNum--];

        if (curr->keyNum == 1 && strcmp(curr->key, key) == 0){
            *n = curr->valueNum;
            ptr = (char**)calloc(*n, sizeof(char*)); 
            temp = curr->dataHead;
            for (j = 0; j < *n; ++j){
                ptr[j] = temp->value;
                temp = temp->next;
            } 
            return ptr;
        }
    }
    return NULL;
}

void mvm_free(mvm** p)
{
    int i;
    mvm* m = *p;
    ht* curr;
    node* temp,* prev;

    for (i = 0; i < HTSIZE; ++i){
        curr = &m->head[i];
        if (curr->hashNum != 0){
            curr->hashNum = curr->keyNum = 0;
            free(curr->key);

            prev = curr->dataHead;
            while (prev){
                temp = prev->next;
                free(prev->value);
                free(prev);
                prev = temp;
            } 
        }
    }
    free(m);
    *p = NULL;
}
