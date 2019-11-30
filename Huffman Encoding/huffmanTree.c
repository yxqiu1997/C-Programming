#include "huffmanTree.h"

int main(int argc, char **argv)
{
    listNode *start, *current, *rightPlace, *temp;
    listNode *firstMin, *secondMin, *top;
    int numOfChars, i;
    FILE *fp;
    char c, code[MAXCODELEN];

    if (argc != 2){
        fprintf(stderr, "ERROR: Invalid Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");
    /* Allocate the first node to the linklist */
    c = getc(fp);
    start = current = allocateNode(c);

    /* Allocate the rest of nodes in order */
    while ((c = getc(fp)) != EOF){
        /* Insert at the end of the linklist */
        if (c >= current->symbol){
            current->next = allocateNode(c);
            current = current->next;            
        }
        /* Insert at the head of the linklist */
        else if (c <= start->symbol){
            temp = allocateNode(c);
            temp->next = start;
            start = temp;
        }
        /* Insert at the middle of the linklist */
        else{
            rightPlace = findPlace(start, c);
            temp = allocateNode(c);
            temp->next = rightPlace->next;
            rightPlace->next = temp;    
        }  
    }
    
    /* Combine identical items in the linklist */
    start = combineItems(start);

    /* Find the number of characters */
    numOfChars = calculateNum(start);

    /* Sort the linklist by frequency from small to large */ 
    sortList(start, numOfChars);
    
    /* Print the linklist */
    /*printList(start);*/

    /* Create a huffman tree */
    while (numOfChars >= 3){
        firstMin = start;
        secondMin = start->next;
        current = createSubtree(firstMin, secondMin);
        start = secondMin->next;
        start = insertSubtree(start, current);
        numOfChars--;    
    }
    top = createSubtree(start, start->next);

    /* print huffman tree */
    i = 0;
    printHuffmanTree(top, code, i);

    fclose(fp);
    return 0;
}

void sortList(listNode* head, int length)
{
    listNode* curr;
    int i, j;

    for (i = 0; i < length - 1; ++i){
        curr = head;
        for (j = 0; j < length - i - 1; ++j){
            if (curr->frequency > curr->next->frequency){
                swapNode(curr, curr->next);
            }
            curr = curr->next;
        }
    }
}

void swapNode(listNode* p1, listNode* p2)
{
    listNode* temp;

    temp = allocateNode(' ');

    temp->frequency = p1->frequency;
    temp->symbol = p1->symbol;

    p1->frequency = p2->frequency;
    p1->symbol = p2->symbol;
    
    p2->frequency = temp->frequency;
    p2->symbol = temp->symbol;

    free(temp);
}

void printHuffmanTree(listNode* node, char* code, int i)
{
    if (node->left != NULL && node->flag == 0){
        code[i] = 0;
        printHuffmanTree(node->left, code, i + 1);
    }
    if (node->right != NULL && node->flag == 0){
        code[i] = 1;
        printHuffmanTree(node->right, code, i + 1);
    }
    if (node->flag == 1){
        if (node->symbol == '\r' || node->symbol == '\n'){
            printf("010: ");
        }
        else{
            printf("'%c': ", node->symbol);
        }
        free(node);
        printCode(code, i);
        return;
    }
}

void printCode(char* code, int num)
{
    int i;

    for (i = 0; i < num; ++i){
        printf("%d", code[i]);
    }
    printf("\n");
}

listNode* insertSubtree(listNode* head, listNode* insertElem)
{
    listNode* curr, *end;

    curr = end = head;
    while (end->next != NULL){
        end = end->next;
    }
    if (insertElem->frequency <= curr->frequency){
        insertElem->next = curr;
        head = insertElem;
        return head; 
    }
    else if (insertElem->frequency >= end->frequency){
        end->next = insertElem;
        return head;
    }
    else{
        for (curr = head; curr != NULL; curr = curr->next){
            if (insertElem->frequency >= curr->frequency && insertElem->frequency <= curr->next->frequency){
                insertElem->next = curr->next;
                curr->next = insertElem;
                return head;
            }
        }
    }
    return head;
}

listNode* createSubtree(listNode* l, listNode* r)
{
    listNode* p;

    p = (listNode*)malloc(sizeof(listNode));
    p->symbol = ' ';
    p->frequency = l->frequency + r->frequency;
    p->flag = 0;
    p->next = NULL;
    p->left = l;
    p->right = r;

    return p;
}

int calculateNum(listNode* head)
{
    int cnt = 0;
    listNode* curr = head;

    while (curr != NULL){
        cnt++;
        curr = curr->next;
    }
    return cnt;
}

listNode* allocateNode(char c)
{
    listNode* p;

    p = (listNode*)malloc(sizeof(listNode));
    if (p == NULL){
        printf("Cannot allocate node to the linklist.\n");
        exit(EXIT_FAILURE);
    }
    p->symbol = c;
    p->frequency = 0;
    p->flag = 1;
    p->next = NULL;

    return p;
}

listNode* findPlace(listNode* head, char c)
{
    listNode* curr;

    curr = head;
    while (curr != NULL){
        if (c >= curr->symbol && c <= curr->next->symbol){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

listNode* combineItems(listNode* head)
{
    int cnt;
    listNode *leftPtr, *rightPtr, *temp;

    leftPtr = rightPtr = head;
    while (leftPtr != NULL){
        cnt = 1;
        rightPtr = rightPtr->next;

        if (rightPtr == NULL){
            leftPtr->frequency = cnt;
            leftPtr->next = rightPtr;
            return head;
        }

        while (leftPtr->symbol == rightPtr->symbol){
            cnt++;
            temp = rightPtr;
            rightPtr = rightPtr->next;
            free(temp);

            if (rightPtr == NULL){
                leftPtr->frequency = cnt;
                leftPtr->next = rightPtr;
                return head;
            }
            leftPtr->frequency = cnt;
            leftPtr->next = rightPtr;
        }
        leftPtr->frequency = cnt;
        leftPtr = rightPtr;
    }
    return head;
}

void printList(listNode* head)
{
    listNode *curr;
    int cnt = 0;

    curr = head;
    while (curr != NULL){
        if (curr->symbol == '\r' || curr->symbol == '\n'){
            printf("010[%d] ", curr->frequency);
        }
        else{
            printf("%c[%d] ", curr->symbol, curr->frequency);
        }
        curr = curr->next;
        cnt++;
    }
    printf("\n");
}

void freeList(listNode* head)
{
    listNode* curr;

    while (head != NULL){
        curr = head;
        head = head->next;
        free(curr);
        curr = NULL;
    }
}
