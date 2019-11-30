#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXCODELEN 20

struct listNode{
    char symbol;
    int frequency;
    int flag;
    struct listNode* left;
    struct listNode* right;
    struct listNode* next;
};
typedef struct listNode listNode;

listNode* allocateNode(char c);
listNode* findPlace(listNode* head, char c);
listNode* combineItems(listNode* head);
int calculateNum(listNode* head);
void printList(listNode* head);
void freeList(listNode* head);
listNode* createSubtree(listNode* l, listNode* r);
listNode* insertSubtree(listNode* head, listNode* insertElem);
void printHuffmanTree(listNode* node, char* code, int i);
void printCode(char* code, int num);
void sortList(listNode* head, int length);
void swapNode(listNode* p1, listNode* p2);