/* Please note that all functions are tested at 71.c */
/* so there isn't any testing at 72.c */
/* The only difference between 71.c and 72.c is that */
/* I use sdlPrint() to replace printBoard() */
#include "72.h" 
#include "neillsdl2.h"

int main(int argc, char **argv)
{
    static boardInfo list[MAXSPACE];
    progParams params;
    int cnt;

    if (argc != 2 || strlen(argv[1]) != TOTALNUM){
        error(InvalidUsage, argv[1]);
        exit(EXIT_FAILURE);
    }

    /* validate if the typed string is legal */
    /* e.g. string with two number one is illegal */
    params = validateParams(argv[1]);
    if (params.flag == 0){
        error(InvalidNumber, argv[1]);
        exit(EXIT_FAILURE);
    }
    if (params.solvable == 0){
        error(NoSolution, argv[1]);
        exit(EXIT_FAILURE);
    }

    /* initial board setup */
    list[0] = string2board(params.string);

    /* main function */
    cnt = eightTile(list);

    /* print movements */
    sdlPrint(list, cnt);

    return 0;
}

void sdlPrint(boardInfo *list, int cnt)
{
    int i, j, num = 0;
    boardInfo *display;
    SDL_Simplewin sw;
    SDL_Rect rectangle;
    fntrow fontdata[FNTCHARS][FNTHEIGHT];
    
    display = (boardInfo*)calloc(MAXSPACE, sizeof(boardInfo));

    /* store child boards that need to be printed in an adverse order */
    do{
        copyBoard(display[num++].board, list[cnt].board);
        cnt = list[cnt].parentLocation;
    }while (cnt != 0);

    copyBoard(display[num].board, list[0].board);

    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;

    Neill_SDL_Init(&sw);
    Neill_SDL_ReadFont(fontdata, "mode7.fnt");

    do{
        for (j = 0; j < N; j++){
            for (i = 0; i < N; i++){     
                Neill_SDL_SetDrawColour(&sw, WHITER, WHITEG, WHITEB);
                 
                rectangle.x = OFFSET + i * RECTSIZE;
                rectangle.y = OFFSET + j * RECTSIZE;
                SDL_RenderDrawRect(sw.renderer, &rectangle);
                    
                if (display[num].board[j][i] != 0){
                    Neill_SDL_DrawChar(&sw, fontdata, display[num].board[j][i] + '0',\
                                            rectangle.x + RECTSIZE / 2, rectangle.y + RECTSIZE / 2);
                }
                else{
                    Neill_SDL_DrawChar(&sw, fontdata, ' ',\
                                            rectangle.x + RECTSIZE / 2, rectangle.y + RECTSIZE / 2);
                }
            }
        }
        num--;

        SDL_Delay(MILLISECONDDELAY);       
        Neill_SDL_UpdateScreen(&sw);
        Neill_SDL_Events(&sw);   

        if (num == -1){
            SDL_Delay(MILLISECONDDELAY);
        }

    }while(!sw.finished && num != -1);

    free(display);
    SDL_Quit();
    atexit(SDL_Quit);
}

int eightTile(boardInfo *list)
{
    int i, j, k, cnt, childrenNum;
    boardInfo children[MAXCHILDREN];

    /* generate and store children boards */
    for (i = 0, cnt = 0; i < MAXSPACE; i++){
        childrenNum = optionNum(list[i].board);

        /* use list[MAXSPACE] to store parent board */
        /* in case of possible changes on original board */
        copyBoard(list[MAXSPACE].board, list[i].board);

        /* generate child boards and store all of them into children[] */
        generateBoard(list[MAXSPACE].board, children);

        /* check if the child board is repeated */
        /* set the flag of repeated board to 0 */
        for (k = 0; k < childrenNum; k++){
            for (j = 0; j <= i; j++){
                if (repeatedBoard(children[k].board, list[j].board)){
                    children[k].flag = 0;
                }
            }
        }

        /* add new child boards to the list */
        for (j = 0; j < childrenNum; j++){
            if (children[j].flag){
                copyBoard(list[++cnt].board, children[j].board);
                list[cnt].parentLocation = i;

                /* if finding the solution, print it */
                if(isSolution(list[cnt].board)){
                    return cnt;
                }
            }
        } 
    }
    return 0;
}

void generateBoard(int board[][N], boardInfo *children)
{
    int a[N][N], b[N][N], c[N][N], d[N][N], temp[N][N];
    int i, j, left = 0, right = 0, up = 0, down = 0;

    initialiseBoard(a, b, c, d);
    initialiseBoard(children[0].board, children[1].board,\
                        children[2].board, children[3].board);
    for (i = 0; i < MAXCHILDREN; i++){
        children[i].flag = 0;
    }

    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            /* in case of changes happen on the board in the stack */
            copyBoard(temp, board);

            /* go right */
            search(j, i, j, i + 1, &right, board, temp, a);
            /* go left */
            search(j, i, j, i - 1, &left, board, temp, b);
            /* go up */
            search(j, i, j - 1, i, &up, board, temp, c);
            /* go down */
            search(j, i, j + 1, i, &down, board, temp, d);
        }
    }

    /* store all child boards into children[] array */
    i = 0;
    store(right, &i, &children[0], a);
    store(left, &i, &children[0], b);
    store(up, &i, &children[0], c);
    store(down, &i, &children[0], d);
}

void store(int direction, int *offset, boardInfo *children, int board[][N])
{
    if (direction){
        copyBoard(children[*offset].board, board);
        children[(*offset)++].flag = 1;
    }
}

void search(int j, int i, int y, int x, int *direction,\
                int board[][N], int temp[][N], int store[][N])
{
    if (validPos(y, x) && !board[y][x]){
        /* if move is permitted, swap the numbers */
        swapNumber(&temp[y][x], &temp[j][i]);
        (*direction)++;
        copyBoard(store, temp);
    }
}

void printBoard(boardInfo *list, int cnt)
{
    boardInfo *display;
    int num = 0, i, j, k;

    display = (boardInfo*)calloc(cnt, sizeof(boardInfo));

    /* store child boards that need to be printed in an adverse order */
    do{
        copyBoard(display[num++].board, list[cnt].board);
        cnt = list[cnt].parentLocation;
    }while (cnt != 0);

    copyBoard(display[num].board, list[0].board);

    for (k = num; k >= 0; k--){
        printf("\nMove %d:\n\n", num - k);
        for (j = 0; j < N; j++){
            for (i = 0; i < N; i++){
                if (display[k].board[j][i] == 0){
                    printf("  ");
                }
                else{
                    printf("%d ", display[k].board[j][i]);
                }
            }
            printf("\n");
        }
        printf("\n------------\n");
    }
    printf("\nTotal Move: %d\n\n", num);

    free(display);
}

void initialiseBoard(int a[][N], int b[][N], int c[][N], int d[][N])
{
    int i, j;

    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            a[j][i] = 0, b[j][i] = 0;
            c[j][i] = 0; d[j][i] = 0;
        }
    }
}

bool isSolution(int board[][N])
{
    int i, j, cnt = 1, temp[N][N];

    copyBoard(temp, board);

    /* find if the child board equals to "123456780" */
    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            if (temp[j][i] == 0){
                temp[j][i] = TOTALNUM;
            }
            if (temp[j][i] != cnt++){
                return false;
            }
        }
    }
    return true;
}

void swapNumber(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

bool validPos(int y, int x)
{
    /* only positions between 0 and 2 are legal */
    if (y >= 0 && x >= 0 && y < N && x < N){
        return true;
    }
    return false;
}

bool repeatedBoard(int a[][N], int b[][N])
{
    int i, j;

    /* find out if two arrays are equivalent */
    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            if (b[j][i] != a[j][i]){
                return false;
            }
        }
    }
    return true;
}

void copyBoard(int des[][N], int src[][N])
{
    int i, j;

    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            des[j][i] = src[j][i];
        }
    }
}

int optionNum(int board[][N])
{
    int i;

    if (board[0][0] == 0 || board[N - 1][N - 1] == 0 ||\
        board[0][N - 1] == 0 || board[N - 1][0] == 0){
        return MAXCHILDREN - 2;
    }
    else{
        for (i = 1; i < N - 1; i++){
            if (board[0][i] == 0 || board[N - 1][i] == 0 ||\
                board[i][0] == 0 || board[i][N - 1] == 0){
                return MAXCHILDREN - 1;
            }
        }
        return MAXCHILDREN;
    }
}

boardInfo string2board(int *str)
{
    int i, j, k = 0;
    boardInfo p;

    /* convert 1D array to 2D array */
    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            p.board[j][i] = str[k++];
        }
    }

    return p;
}

progParams validateParams(char *str)
{
    progParams p;
    int i, hist[TOTALNUM] = {0};

    p.flag = 1, p.solvable = 1;

    /* convert type char to type int */
    for (i = 0; i < TOTALNUM; i++){
        if (str[i] == ' '){
            p.string[i] = '0' - '0';
        }
        else{
            p.string[i] = str[i] - '0';
        }
    }

    /* validate if the string only contains number 0 to 8 */
    for (i = 0; i < TOTALNUM; i++){
        if (p.string[i] >= 0 && p.string[i] < TOTALNUM){
            hist[p.string[i]]++;
        }
        else{
            p.flag = 0;
            return p;
        }
    }

    /* test if all numbers appear only once */
    for (i = 0; i < TOTALNUM; i++){
        if (hist[i] != 1){
            p.flag = 0;
            return p;
        }
    }

    /* test if the initial board is solvable */
    /* formula for determining solvability and the introduction of inversions can be seen at */
    /* http://www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html */
    if (!solutionExists(p.string)){
        p.solvable = 0;
    }

    return p;
}

bool solutionExists(int *init)
{
    int i, j, invertNum = 0;

    for (i = 0; i < TOTALNUM - 1; i++){
        for (j = i + 1; j < TOTALNUM; j++){
            if (init[i] && init[j] && init[i] > init[j]){
                invertNum++;
            }
        }
    }

    if (invertNum % 2 == 0){
        return true;
    }
    else{
        return false;
    }
}

void error(ErrCode errcode, char *str)
{
    switch (errcode){
        case InvalidUsage:
            fprintf(stderr, "Invalid usage: %s\n", str);
            break;
        case InvalidNumber:
            fprintf(stderr, "Invalid number in %s\n", str);
            break;
        case NoSolution:
            fprintf(stderr, "There is no solution in %s\n", str);
            break;
    }
}