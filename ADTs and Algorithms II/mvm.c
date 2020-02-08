#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mvm.h"

#define SPECIALCHARNUM 5 
#define MAXSIZE 200

mvm* mvm_init(void)
{
    mvm* map;
    mvmcell* sentinelNode;

    map = (mvm*)malloc(sizeof(mvm));
    if (map == NULL){
        ON_ERROR("Creation of MultiValue Map failed.\n");
    }

    sentinelNode = (mvmcell*)malloc(sizeof(mvmcell));
    if (sentinelNode == NULL){
        ON_ERROR("Creation of the sentinel node failed.\n");
    }

    map->head = sentinelNode;
    sentinelNode->next = NULL;
    map->numkeys = 0;

    return map;
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
    mvmcell* node;

    if (m == NULL || key == NULL || data == NULL){
        return;
    }

    node = (mvmcell*)malloc(sizeof(mvmcell));
    node->key = (char*)calloc(MAXSIZE, sizeof(char));
    node->data = (char*)calloc(MAXSIZE, sizeof(char));
    if (node->key == NULL || node->data == NULL || node == NULL){
        ON_ERROR("Failed to insert the node to the linklist.\n");
    }

    strcpy(node->key, key);
    strcpy(node->data, data);

    ++m->numkeys;
    if (m->numkeys == 1){
        
        m->head->next = node;
        node->next = NULL;
        return;
    }
    node->next = m->head->next;
    m->head->next = node;
    
    return;
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
    int sum = 0, i = 0;
    mvmcell* curr;
    char* str;

    curr = m->head->next;
    while (curr){
        sum += SPECIALCHARNUM;
        sum += (int)strlen(curr->key) + 1;
        sum += (int)strlen(curr->data) + 1;
        curr = curr->next;
    }

    str = (char*)calloc(sum, sizeof(char));

    curr = m->head->next;
    while (curr){
        i += sprintf(str + i, "[%s](%s) ", curr->key, curr->data);
        curr = curr->next;
    }

    return str;
}

/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
    mvmcell* prev, *curr;

    if (key == NULL || m == NULL || strcmp(key, "") == 0){
        return;
    }

    --m->numkeys;
    prev = m->head;
    curr = prev->next;
    while (curr){
        if (strcmp(curr->key, key) == 0){
            prev->next = curr->next;
            free(curr->key);
            free(curr->data);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    ON_ERROR("Failed to find the key that is supposed to be deleted.\n");
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
    int i;
    mvmcell* curr = m->head->next;

    for (i = 0; i < m->numkeys; ++i){
        if (strcmp(key, curr->key) == 0){
            return curr->data;
        }
        curr = curr->next;
    }

    return NULL;
}

/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
    int cnt = 0, i;
    mvmcell* curr = m->head->next;
    char** pointerArray;

    while (curr){
        if (strcmp(curr->key, key) == 0){
            ++cnt;
        }
        curr = curr->next;
    }
    pointerArray = (char**)calloc(cnt, sizeof(char*));

    i = 0;
    curr = m->head->next;
    while (curr != NULL){
        if (strcmp(curr->key, key) == 0){
            pointerArray[i++] = curr->data;
        }
        curr = curr->next;
    }
    *n = cnt;
    
    return pointerArray;
}

/* Free & set p to NULL */
void mvm_free(mvm** p)
{
    mvm* map = *p;
    mvmcell* curr,* temp;

    curr = map->head->next;
    while (curr){
        temp = curr->next;
        free(curr->key);
        free(curr->data);
        free(curr);
        curr = temp;
    }

    free(map->head);
    free(map);
    *p = NULL;
}
