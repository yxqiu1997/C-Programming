#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

/* the number of possible combinations is 9!=362880*/
#define MAXSPACE 362880

#define N 3
#define TOTALNUM (N * N)
#define MAXCHILDREN 4

#define RECTSIZE 100
#define MILLISECONDDELAY 2000
#define OFFSET 100
#define WHITER 255
#define WHITEG 255
#define WHITEB 255

struct boardInfo{
    int board[N][N];
    int parentLocation;
    int flag;
};
typedef struct boardInfo boardInfo;

struct progParams{
    int string[TOTALNUM];
    int flag;
    int solvable;
};
typedef struct progParams progParams;

typedef enum ErrCode{InvalidUsage, InvalidNumber, NoSolution} ErrCode;

/* functions about board generation */
void error(ErrCode errcode, char *str);
progParams validateParams(char *str);
bool solutionExists(int *init);
boardInfo string2board(int *str);
int optionNum(int board[][N]);
void copyBoard(int des[][N], int src[][N]);
bool repeatedBoard(int a[][N], int b[][N]);
bool validPos(int y, int x);
void swapNumber(int *a, int *b);
bool isSolution(int board[][N]);
void generateBoard(int board[][N], boardInfo *children);
int eightTile(boardInfo *list);
void search(int j, int i, int y, int x, int *direction,\
                int board[][N], int temp[][N], int store[][N]);
void store(int direction, int *offset, boardInfo *children, int board[][N]);
void initialiseBoard(int a[][N], int b[][N], int c[][N], int d[][N]);

/* function about sdl */
void sdlPrint(boardInfo *list, int cnt);