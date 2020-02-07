#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <assert.h>
#define SIZE 5
#define LEFT 9
#define RIGHT 8
#define MAXDEPTH 20
#define LENGTH 100
#define WIDTH 100

typedef struct node
{
    char c[SIZE];
    int code[MAXDEPTH];
    int depth;
    struct node *left;
    struct node *right;
}Node;

Node *readIn(char *s);
int findFlag(char *s);
Node *makeTreeNode(char *num);
void divLeaf(char *lefts, char *rights, char *s);
void copy(char *lefts, char *s, int begin, int end);
int strLenth(char *s);
void codeCount(Node *head, int len);
int widthCount(Node *head);
void printTree(Node *head, char array[LENGTH][WIDTH], int i, int j);
void writeIn(Node *head, char **map, int len);
void findDepth(Node *treeHead);

int main(void)
{
    char *s = "00001(00002(00003(00004(00005(*)(00009))(00007(*)(00008)))(00006))(*))(00010(00011(00012)(*))(00013(*)(*)))";
    Node *treehead = readIn(s);
    int i, j;
    char map[LENGTH][WIDTH];
    codeCount(treehead, 0);
    findDepth(treehead);
    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < WIDTH; j++){
            map[i][j] = ' ';
        }
    }
    printTree(treehead, map, 0, 0);
    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < WIDTH; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    return 0;
}

void findDepth(Node *treeHead)
{
    static depth = 1;
    if(treeHead == NULL) {
        return;
    }
    treeHead->depth = depth;
    depth++;
    findDepth(treeHead->left);
    findDepth(treeHead->right);
    depth--;
}

void printTree(Node *head, char array[LENGTH][WIDTH], int i, int j)
{
    int k= 0;
    int m = 0;
    if (head == NULL) {
        return;
    }
    for (k = 0; k < SIZE; k++) {
        array[i][j + k] = head->c[k];
    }
    if (head->left != NULL) {
        for (k = 1; k < 10 / head->depth; k++){
            array[i + k][j] = '|';
        }
    }
    if (head->right != NULL) {
        for (k = SIZE; k < SIZE + SIZE; k++){
            array[i][j + k] = '-';
        }
    }

    printTree(head->left, array, i + 10 / head->depth, j);
    printTree(head->right, array, i, j + 10);
}

void writeIn(Node *head, char **map, int len)
{
    int i, j;
    if(head != NULL) {
        for (i = 0; i < SIZE; i++) {
            *(*(map + len * 2) + i) = head->c[i];
        }
        if (head->left) {
            for (i = 0; i < SIZE; i++) {
                *(*(map + 2 * len + 1) + 2) = '|';
            }
        }
        writeIn(head->left, map, len + 1);
        if (head->right) {
            for (i = 0; i < SIZE; i++) {
                *(*(map + 2 * len) + SIZE) = '-';
            }
            for (i = 0; i < SIZE; i++) {
                *(*(map + len * 2) + i) = head->c[i];
            }
        }
    }
}

int widthCount(Node *head)
{
    static int width = 1;
    int temp;
    if (head != NULL) {
        if (head->right != NULL) {
            width++;
        }
        temp = widthCount(head->left);
        width = (width < temp) ? temp : width;
        temp = widthCount(head->right);
        width = (width < temp) ? temp : width;
    }
    return width;
}


void codeCount(Node *head, int len)
{
    static int a[MAXDEPTH];
    int i;
    if (head != NULL) {
        for (i = 0; i < MAXDEPTH; i++) {
            head->code[i] = a[i];
        }
        a[len] = LEFT;
        codeCount(head->left, len+1);
        a[len] = RIGHT;
        codeCount(head->right, len+1);
        a[len] = 0;
    }
}

Node *readIn(char *s)
{
    Node *head;
    char num[5] = {'\0'};
    int i;
    char *lefts = (char *)malloc(strLenth(s) * sizeof(char));
    char *rights = (char *)malloc(strLenth(s) * sizeof(char));
    if (s[0] == '*') {
        return NULL;
    }
    if (findFlag(s)) {
        for (i =0 ; (s[i] >= '0' && s[i] <= '9'); i++) {
            num[i] = s[i];
        }
        head = makeTreeNode(num);
        divLeaf(lefts, rights, s);
        head->left = readIn(lefts);
        head->right = readIn(rights);
    }
    else {
        head = makeTreeNode(s);
    }
    return head;
}

void divLeaf(char *lefts, char *rights, char *s)
{
    int begin = 0, end, cnt = 0;
    while (s[begin] != '(') {
        begin++;
    }
    for (end = 1; !(s[end - 1] == ')' && cnt == 0); end++) {
        if (s[end] == '(') {
            cnt++;
        }
        if (s[end] == ')') {
            cnt--;
        }
    }
    copy(lefts, s, begin, end);
    lefts[end - begin - 2] = '\0';
    begin = end;
    end = strLenth(s);
    copy(rights, s, begin, end);
    rights[end - begin - 2] = '\0';
}

int strLenth(char *s)
{
    int cnt = 0;
    while ((s[cnt] >= '0' && s[cnt] <= '9') || s[cnt] == '(' || s[cnt] == ')' || s[cnt] == '*') {
        cnt++;
    }
    return cnt;
}

void copy(char *leaf, char *s, int begin, int end)
{
    int i;
    for (i = begin + 1; i < end - 1; i++) {
        leaf[i - begin - 1] = s[i];
    }
}

int findFlag(char *s)
{
    int i, length = strlen(s);
    for (i = 0; i < length; i++) {
        if (s[i] == '(' || s[i] == ')') {
            return 1;
        }
    }
    return 0;
}

Node *makeTreeNode(char *num)
{
    Node *head = (Node *)calloc(1, sizeof(Node));
    int i, lenth = strLenth(num);
    assert(head != NULL);
    for (i = 0; i < lenth; i++) {
        head->c[i] = num[i];
    }
    return head;
}