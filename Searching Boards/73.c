/* Please note that only one function, allocateNode(), will be tested at 73.c */
/* since other functions are nearly the same as those at 71.c */
/* Outcome of 73.c can be printed by either plain-text and SDL */
/* but statements of plain-text printing have been commented out */
#include "73.h"
#include "neillsdl2.h"

int main(int argc, char **argv)
{
    progParams params, *display;
    int num, space;
    Node *head;

    /* test functions */
    test();

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

    /* set the head node */
    params = string2board(params.string);
    params.parentLocation = -1;
    head = allocateNode(params);

    /* main function */
    space = eightTile(head);

    display = (progParams*)calloc(space, sizeof(progParams));

    /* store boards in the solution */
    num = storeSolution(display, space - 1, head);

    /* print */
    /*printBoards(display, num);*/
    sdlPrint(display, num);

    free(display);
    freeMemory(head);

    return 0;
}

void sdlPrint(progParams *display, int num)
{
    int i, j;
    SDL_Simplewin sw;
    SDL_Rect rectangle;
    fntrow fontdata[FNTCHARS][FNTHEIGHT];

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

    SDL_Quit();
    atexit(SDL_Quit);
}

int storeSolution(progParams *display, int size, Node *head)
{
    int i, num = 0;
    Node *curr;

    do{
        curr = head;
        for (i = 0; i < size; i++){
            curr = curr->next;
        } 
        copyBoard(display[num++].board, curr->info.board);
        size = curr->info.parentLocation;
    }while (size != -1);
    
    return num;
}

void printBoards(progParams *display, int num)
{
    int i, j, k;

    for (k = num - 1; k >= 0; k--){
        printf("\nMove %d:\n\n", num - k - 1);
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
    printf("\nTotal Move: %d\n\n", num - 1);
}

Node *allocateNode(progParams element)
{
    Node *p;

    p = (Node*)malloc(sizeof(Node));

    if (p == NULL){
        error(CantAllocate, "NULL");
        exit(EXIT_FAILURE);
    }

    copyBoard(p->info.board, element.board);

    p->info.parentLocation = element.parentLocation;
    p->next = NULL;

    return p;
}

void error(ErrCode errcode, char *str)
{
    switch (errcode){
        case CantAllocate:
            fprintf(stderr, "Can't allocate node: %s pointers.\n", str);
            break;
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

void copyBoard(int des[][N], int src[][N])
{
    int i, j;

    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            des[j][i] = src[j][i];
        }
    }
}

progParams validateParams(char *str)
{
    progParams p;
    int i, hist[TOTALNUM] = {0};

    p.flag = 1, p.solvable = 1;

    /* convert space to 0 */
    for (i = 0; i < TOTALNUM; i++){
        if (str[i] == ' '){
            p.string[i] = '0' - '0';
        }
        else{
            p.string[i] = str[i] - '0';
        }
    }

    /* validate the string and only number from 0 to 9 and the space are permitted */
    for (i = 0; i < TOTALNUM; i++){
        if (p.string[i] >= 0 && p.string[i] < TOTALNUM){
            hist[p.string[i]]++;
        }
        else{
            p.flag = 0;
            return p;
        }
    }

    /* test if the numbers appear only once */
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

progParams string2board(int *str)
{
    int i, j, k = 0;
    progParams p;

    /* convert 1D array to 2D array */
    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            p.board[j][i] = str[k++];
        }
    }

    return p;
}

int eightTile(Node *head)
{
    Node *parentptr, *childptr, *curr;
    int cnt = 0, i, childrenNum, num = 1;
    progParams children[MAXCHILDREN];

    curr = head;
    parentptr = head, parentptr->flag = 1;


    while (parentptr != NULL){

        childrenNum = optionNum(parentptr->info.board);

        generateBoard(parentptr->info.board, children);

        /* find repeated boards and set their flags to zero */
        for (i = 0; i < childrenNum; i++){
            childptr = head;
            while (childptr != NULL){
                if (repeatedBoard(children[i].board, childptr->info.board)){
                    children[i].flag = 0;
                }
                childptr = childptr->next;
            }
        }

        /* find the solution */
        for (i = 0; i < childrenNum; i++){

            /* link new boards */
            if (children[i].flag){
                children[i].parentLocation = cnt;
                curr->next = allocateNode(children[i]);
                curr = curr->next;
                num++;

                if (isSolution(children[i].board)){          
                    return num;
                }
            }
        }
        parentptr = parentptr->next;
        cnt++;
    }
    return 0;
}

void generateBoard(int board[][N], progParams *children)
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

void store(int direction, int *offset, progParams *children, int board[][N])
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

bool isSolution(int board[][N])
{
    int i, j, cnt = 1, temp[N][N];

    copyBoard(temp, board);

    /* check if the child board equals to "12345678 " */
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

void freeMemory(Node *head)
{
    Node *p;

    while (head != NULL){
        p = head;
        head = head->next;
        free(p);
        p = NULL;
    }
}

void test(void)
{
    progParams p;
    Node *q, *q1;

    /* test for *allocateNode() */
    p.parentLocation = 100;
    q = allocateNode(p);
    p.parentLocation = 200;
    q1 = allocateNode(p);
    q->next = q1;
    assert(q->info.parentLocation == 100);
    assert(q->next == q1);
    assert(q1->info.parentLocation == 200);
    assert(q1->next == NULL);
}