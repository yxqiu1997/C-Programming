#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
/*#include "mvm.h"*/
#include "fmvm.h"

#define MAXSIZE 200
#define ASSUMEVALUE 3
#define FLAGINDEX 1
#define NUMINDEX 2
#define WORDINDEX 3
#define OFFSET 2
#define WHOLEFLAG -1

struct dictionaryType{
    char* word;
    char* phonemes;
    int flag;
};
typedef struct dictionaryType dic;

void test(void);
int getPhonemesNum(int num, char** string);
void storeData2Map(mvm* map1, mvm* map2, int phonemesNum);
int getNumberSignLocation(char* string);
void storeString(dic* pargma, int numSignLocation, char* string);
int getSpaceLocation(dic* pargma, int phonemesNum);
void getFinalPhonemes(char* editedArray, char* string, int spaceLocation);
void searchAndPrint(mvm* map1, mvm* map2, int totalNum, char** string);

int main(int argc, char** argv)
{
    int phonemesNum;
    mvm* map1,* map2;

    /* Test function */
    test();

    /*Get the number of phonemes we need from command line*/
    phonemesNum = getPhonemesNum(argc, argv);

    /* Initialise map1 and map2 */
    map1 = mvm_init();
    map2 = mvm_init();

    /* Read data from the file */
    storeData2Map(map1, map2, phonemesNum);

    /* Search and print */
    searchAndPrint(map1, map2, argc, argv);

    /* Free two maps */
    mvm_free(&map1);
    mvm_free(&map2);
    
    return 0;
}

void searchAndPrint(mvm* map1, mvm* map2, int totalNum, char** string)
{
    char* key,** value;
    int n, i, j, beginner;

    /* -n flag isn't given */
    if (strcmp(string[FLAGINDEX], "-n") != 0){
        beginner = FLAGINDEX;
    }
    else{
        beginner = WORDINDEX;
    }

    for (i = beginner; i < totalNum; ++i){
        /* Don't have this item in the dictionary */
        /* Or the number of word's syllables is less than the value given from the command line */
        /* Search */
        key = mvm_search(map1, string[i]);
        if (key == NULL){
            printf("%s: Invalid Word (Doesn't exist or lack of syllables).\n", string[i]);
        }
        else{
            /* MultiSearch */
            value = mvm_multisearch(map2, key, &n);

            /* Remove \n from the string */
            key[strlen(key) - 1] = '\0';
            printf("%s (%s): ", string[i], key);

            /* Print */        
            for (j = 0; j < n; ++j){
                printf("%s ", value[j]);
            }
            printf("\n");

            free(value);
        }
    }
}

void storeData2Map(mvm* map1, mvm* map2, int phonemesNum)
{
    FILE* fp;
    dic* pargma;
    char* str,* editedArray;
    int numSignLocation, spaceLocation;

    /* Open the file */
    if ((fp = fopen("cmudict.txt", "r")) == NULL){
        ON_ERROR("Can't find the dictionary file.\n");
    }

    str = (char*)calloc(MAXSIZE, sizeof(char));
    pargma = (dic*)malloc(sizeof(dic));

    while (fgets(str, MAXSIZE, fp) != NULL){
        /* Malloc memory */ 
        pargma->word = (char*)calloc(MAXSIZE, sizeof(char));
        pargma->phonemes = (char*)calloc(MAXSIZE, sizeof(char));

        pargma->flag = 1;
        /* Get the location of the number sign */
        numSignLocation = getNumberSignLocation(str);

        /* Store strings seperated by the number sign */
        storeString(pargma, numSignLocation, str);

        /* Get the location of the specific space */
        spaceLocation = getSpaceLocation(pargma, phonemesNum);

        /* Get final phonemes string */
        editedArray = (char*)calloc(MAXSIZE, sizeof(char));
        getFinalPhonemes(editedArray, pargma->phonemes, spaceLocation);

        /* If the phonemes number is less than the value given from the command line */
        /* give up this item and read next item */
        if (pargma->flag == 1){
            mvm_insert(map1, pargma->word, editedArray);
            /*printf("%s %s\n", pargma->word, editedArray);*/
            mvm_insert(map2, editedArray, pargma->word);
        }

        /* Free memory */
        free(pargma->word);
        free(pargma->phonemes);
        free(editedArray);
    }

    free(pargma);
    free(str);
    fclose(fp);
}

void getFinalPhonemes(char* editedArray, char* string, int spaceLocation)
{
    unsigned int i, j;

    for (i = spaceLocation + 1, j = 0; i < strlen(string); ++i){
        editedArray[j++] = string[i];
    }
    editedArray[i] = '\0';
}

int getSpaceLocation(dic* pargma, int phonemesNum)
{
    int i, cnt = 0;

    for (i = (int)strlen(pargma->phonemes) - 1; i >= 0; --i){
        if (pargma->phonemes[i] == ' '){
            cnt++;
        }
        if (cnt == phonemesNum){
            return i;
        }
    }

    if (phonemesNum - cnt != 1){
        pargma->flag = 0;
    }
    return WHOLEFLAG;
}

void storeString(dic* pargma, int numSignLocation, char* string)
{
    int i, j;

    for (i = 0, j = 0; i < (int)strlen(string) - 1; ++i){
        if (i < numSignLocation){
            pargma->word[i] = string[i];
        }
        if (i > numSignLocation){
            pargma->phonemes[j++] = string[i];
        }
    }
    pargma->word[numSignLocation] = '\0';
    pargma->phonemes[j] = '\0';
}

int getNumberSignLocation(char* string)
{
    unsigned int i;

    for (i = 0; i < strlen(string); ++i){
        if (string[i] == '#'){
            return i;
        }
    }
    ON_ERROR("Invalid items in cumdict.txt\n");
}

int getPhonemesNum(int num, char** string)
{
    int phonemesNum;

    if (num < 2){
        ON_ERROR("Invalid Usage.\n");
    }
    /* If no -n flag is given */
    /* the value 3 is assumed */
    if (strcmp(string[FLAGINDEX], "-n") != 0){
        phonemesNum = ASSUMEVALUE;
    }
    else{
        phonemesNum = atoi(string[NUMINDEX]);
    }
    return phonemesNum;
}

void test(void)
{
    char** string,* word,* edit;
    int i;
    dic* para;

    /* Test for getPhonemesNum() */
    string = (char**)calloc(20, sizeof(char*));
    for (i = 0; i < 6; ++i){
        string[i] = (char*)calloc(20, sizeof(char));
    }
    strcpy(string[0], "./homophones");
    strcpy(string[1], "BOY");
    assert(getPhonemesNum(2, string) == 3);

    strcpy(string[0], "./homophones");
    strcpy(string[1], "-n");
    strcpy(string[2], "2");
    strcpy(string[3], "BOY");
    assert(getPhonemesNum(2, string) == 2);

    for (i = 0; i < 6; ++i){
        free(string[i]);
    }
    free(string);

    /* Test for getNumberSignLocation() */
    word = (char*)calloc(20, sizeof(char));
    strcpy(word, "STANO#S T AA1 N OW0");
    assert(getNumberSignLocation(word) == 5);
    
    /* Test for storeString() */
    para = (dic*)malloc(sizeof(dic));
    para->phonemes = (char*)calloc(20, sizeof(char));
    para->word = (char*)calloc(20, sizeof(char));
    storeString(para, 5, word);
    assert(para->word[0] == 'S');
    assert(para->word[2] == 'A');
    assert(para->word[4] == 'O');

    assert(para->phonemes[0] == 'S');
    assert(para->phonemes[1] == ' ');
    assert(para->phonemes[3] == ' ');
    assert(para->phonemes[5] == 'A');

    /* Test for getSpaceLocation() */
    assert(getSpaceLocation(para, 2) == 7);
    assert(getSpaceLocation(para, 3) == 3);
    assert(getSpaceLocation(para, 5) == -1);
    
    /* Test for getFinalPhonemes() */
    edit = (char*)calloc(20, sizeof(edit));
    getFinalPhonemes(edit, para->phonemes, 7);
    assert(edit[0] == 'N');
    assert(edit[1] == ' ');
    assert(edit[2] == 'O');
    assert(edit[3] == 'W');

    free(para->word);
    free(para->phonemes);
    free(para);
    free(edit);
    free(word);
}
