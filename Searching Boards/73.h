#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#define N 3
#define TOTALNUM 9
#define MAXCHILDREN 4

#define RECTSIZE 100
#define MILLISECONDDELAY 2000
#define OFFSET 100
#define WHITER 255
#define WHITEG 255
#define WHITEB 255

struct progParams{
    int string[TOTALNUM];
    int board[N][N];
    int parentLocation;
    int flag;
    int solvable;
};
typedef struct progParams progParams;

struct node{
    progParams info;
    int flag;
    struct node *next;
};
typedef struct node Node;

typedef enum ErrCode{CantAllocate, InvalidUsage,\
                        InvalidNumber, NoSolution} ErrCode;

/* function about linklist */
Node *allocateNode(progParams element);
void freeMemory(Node *head);

/* function about printing error information */
void error(ErrCode errcode, char *str);

/* functions about board generation */
void copyBoard(int des[][N], int src[][N]);
progParams validateParams(char *str);
bool solutionExists(int *init);
progParams string2board(int *str);
int eightTile(Node *head);
void generateBoard(int board[][N], progParams *children);
void store(int direction, int *offset, progParams *children, int board[][N]);
void search(int j, int i, int y, int x, int *direction,\
                int board[][N], int temp[][N], int store[][N]);
bool isSolution(int board[][N]);
bool repeatedBoard(int a[][N], int b[][N]);
void swapNumber(int *a, int *b);
bool validPos(int y, int x);
void initialiseBoard(int a[][N], int b[][N], int c[][N], int d[][N]);
int optionNum(int board[][N]);
int storeSolution(progParams *display, int size, Node *head);
void printBoards(progParams *display, int num);
void test(void);

/* function about sdl */
void sdlPrint(progParams *display, int num);