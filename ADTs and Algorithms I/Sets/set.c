#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "set.h"

/* Create empty set */
set* set_init(void)
{
    set* s;

    s = (set*)malloc(sizeof(set));
    if (s == NULL){
        ON_ERROR("ERROR: Creation of Set Failed\n");
    }
    s->ua = arr_init();
    s->sz = 0;

    return s;
}

/* Create new set, copied from another */
set* set_copy(set* s)
{
    set* newSet;

    newSet = set_init();
    newSet->ua->pz = s->ua->pz;
    newSet->sz = s->sz;
    memcpy(newSet->ua->data, s->ua->data, s->ua->pz);

    return newSet;
}
/* Create new set, copied from an array of length n*/
set* set_fromarray(arrtype* a, int n)
{
    set* newSet;
    int i, j;

    newSet = set_init();
    newSet->ua->data = a;
    newSet->sz = n;

    for (i = 0; i < n - 1; ++i){
        for (j = i + 1; j < n; ++j){
            if (a[i] == a[j]){
                n--;
            }
        }
    }
    newSet->sz = n;

    return newSet;
}

/* Basic Operations */
/* Add one element into the set */
void set_insert(set* s, arrtype l)
{
    set* newSet;

    if (s == NULL){
        newSet = set_init();
        s = newSet;
    }
    arr_set(s->ua, s->ua->pz + 1, l);
    if (!set_contains(s, l)){
        s->sz++;
    }
}

/* Return size of the set */
int set_size(set* s)
{
    if (s == NULL){
        return 0;
    }
    return s->sz;
}

/* Returns true if l is in the array, false elsewise */
int set_contains(set* s, arrtype l)
{
    int i;

    if (s == NULL){
        return false;
    }
    for (i = 0; i < s->ua->pz; ++i){
        if (s->ua->data[i] == l){
            return true;
        }
    }
    return false;
}

/* Remove l from the set (if it's in) */
void set_remove(set* s, arrtype l)
{
    int i, cnt = 0;
    arr* newArr;

    if (s == NULL){
        return;
    }
    if (!set_contains(s, l)){
        ON_ERROR("The element doesn't exist in the set");
    }
    newArr = arr_init();
    s->sz--;
    newArr->data = (arrtype*)calloc(s->ua->pz, sizeof(arrtype));
    for (i = 0; i < s->ua->pz; ++i){
        if (s->ua->data[i] != l){
            newArr->data[cnt++] = s->ua->data[i];
        }
    }
    free(s->ua->data);
    s->ua->data = (arrtype*)calloc(cnt, sizeof(arrtype));
    memcpy(s->ua->data, newArr->data, cnt);
    s->ua->pz = cnt;

    free(newArr);
}

/* Remove one element from the set - there's no
   particular order for the elements, so any will do */
arrtype set_removeone(set* s)
{
    arrtype unfortunateElem;
    int *newArray, i;
   
    newArray = (int*)calloc(s->ua->pz - 1, sizeof(int));
    unfortunateElem = s->ua->data[s->ua->pz - 1];
    memcpy(newArray, s->ua->data, s->ua->pz - 1);

    /*
    free(s->ua->data);
    s->ua->data = (int*)calloc(s->ua->pz - 1, sizeof(int));
    */
    printf("%d\n", s->ua->pz);
    for (i = 0; i < 4; i++){
        printf("%d ", s->ua->data[i]);
    }
    printf("\n");

    for (i = 0; i < 4; i++){
        printf("%d ", newArray[i]);
    }

    s->ua->data = newArray;
    
    if (!set_contains(s, unfortunateElem)){
        --s->sz;
    }

    return unfortunateElem;
}

/* Operations on 2 sets */
/* Create a new set, containing all elements from s1 & s2 */
set* set_union(set* s1, set* s2)
{
    set* newSet;
    arrtype *newArr;
    int i, j;

    if (s1 == NULL && s2 == NULL){
        newSet = set_init();
        return newSet;
    }

    if (s1 == NULL){
        s1 = set_init();
        newSet = set_copy(s2);
        return newSet;
    }
    if (s2 == NULL){
        s2 = set_init();
        s2->ua->pz = 0;
        newSet = set_copy(s1);
        return newSet;
    }
    
    newSet = set_init();
    newArr = (arrtype*)calloc(s1->ua->pz + s2->ua->pz, sizeof(arrtype));
    for (i = 0; i < s1->ua->pz; ++i){
        newArr[i] = s1->ua->data[i];
    }
    for (i = 0; i < s2->ua->pz; ++i){
        newArr[i + s1->ua->pz] = s2->ua->data[i];
    }
    newSet->sz = newSet->ua->pz = s1->ua->pz + s2->ua->pz;
    memcpy(newSet->ua->data, newArr, newSet->ua->pz);
    free(newArr);

    for (i = 0; i < newSet->ua->pz - 1; ++i){
        for (j = i + 1; j < newSet->ua->pz; ++j){
            if (newSet->ua->data[i] == newSet->ua->data[j]){
                newSet->sz--;
            }
        }
    }
    return newSet;

}

/* Create a new set, containing all elements in both s1 & s2 */
set* set_intersection(set* s1, set* s2)
{
    int i, j, cnt = 0, *newArray;
    set* newSet;

    newSet = set_init();
    newArray = (int*)calloc(s1->ua->pz + s2->ua->pz, sizeof(int));
    for (i = 0; i < s1->ua->pz; ++i){
        for (j = 0; j < s2->ua->pz; ++j){
            if (s1->ua->data[i] == s2->ua->data[j]){
                newArray[cnt] = s1->ua->data[i];
            }
        }
    }
    free(newSet->ua->data);
    newSet->ua->data = (arrtype*)calloc(cnt, sizeof(arrtype));
    memcpy(newSet->ua->data, newArray, cnt);
    newSet->ua->pz = newSet->sz = cnt;
    return newSet;
}

/* Finish up */
/* Clears all space used, and sets pointer to NULL */
void set_free(set** s)
{
    set* a = *s;

    if (s == NULL){
        return;
    }
    free(a->ua);
    free(a);
    *s = NULL;
}
