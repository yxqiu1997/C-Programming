#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "neillncurses.h"

/* The number of rows and columns in the grid */
#define ROW 40
#define COL 40

/* Time for delay */
#define DELAY_TIME 100.0

/* Define Error Code */
#define INVALID_FILE 0
#define INVALID_USAGE 1
#define INVALID_CHARACTER 2

/* Enumerate four states of the element */
typedef enum state{initial, empty, head, tail, copper} state;

void test(void);
void initialize_array(state oldState[ROW][COL], state newState[ROW][COL]);
void ErrInfo(int ErrCode);
void file_to_array(state oldState[ROW][COL], char *filename);
state char_to_state(char c);
void color_print(state oldState[ROW][COL], state newState[ROW][COL]);
void change_elements(state oldState[ROW][COL], state newState[ROW][COL], char display[ROW][COL]);
state change_by_rules(state oldState[ROW][COL], int j, int i);
int find_heads(state oldState[ROW][COL], int j, int i);
char state_to_char(state s);


int main(int argc, char **argv)
{
    state oldArray[ROW][COL], newArray[ROW][COL];

    /* Test functions */
    test();

    /* Initialize arrays that store data */
    initialize_array(oldArray, newArray);

    if (argc == 2){
        file_to_array(oldArray, argv[1]);
        color_print(oldArray, newArray);
    }
    else{
        ErrInfo(INVALID_USAGE);
        exit(EXIT_FAILURE);
    }
    
    return 0;
}


void initialize_array(state oldState[ROW][COL], state newState[ROW][COL])
{
    int i, j;
    for (j = 0; j < ROW; j++){
        for (i = 0; i < COL; i++){
            oldState[j][i] = initial;
            newState[j][i] = initial;
        }
    }

}


void file_to_array(state oldState[ROW][COL], char *filename)
{
    FILE *ifp;
    int i, j;
    char c;

    if ((ifp = fopen(filename, "r")) == NULL){
        ErrInfo(INVALID_FILE);
        exit(EXIT_FAILURE);
    }

    /* Only letters and space are allowed to enter the array */
    /* Back one space when \n appears */
    /* When the file contains unexpected letters, print error information */
    for (j = 0; j < ROW; j++){
        for (i = 0; i < COL; i++){
            c = getc(ifp);
            if ((isalpha(c)) || (c == ' ')){
                oldState[j][i] = char_to_state(c);
            }
            else{
                i--;
            }
        }
    }
    fclose(ifp);

}


void color_print(state oldState[ROW][COL], state newState[ROW][COL])
{
    NCURS_Simplewin sw;
    char displayArray[ROW][COL];

    Neill_NCURS_Init(&sw);
    /* Use red, blue and yellow to represent character t, H and c respectively */
    Neill_NCURS_CharStyle(&sw, "t", COLOR_RED, COLOR_RED, A_NORMAL);
    Neill_NCURS_CharStyle(&sw, "H", COLOR_BLUE, COLOR_BLUE, A_NORMAL);
    Neill_NCURS_CharStyle(&sw, "c", COLOR_YELLOW, COLOR_YELLOW, A_NORMAL);

    do{
        /* Change elements based on given rules */
        /* And store outcomes to new array */
        change_elements(oldState, newState, displayArray);

        /* Print array */
        Neill_NCURS_PrintArray(&displayArray[0][0], ROW, COL, &sw);

        /* Delay in 1/100s of a second */
        Neill_NCURS_Delay(DELAY_TIME);

        /* Test for mouse click, or ESC key */
        Neill_NCURS_Events(&sw);

        /* Copy new array into the old array and begin next loop */
        memcpy(oldState, newState, ROW * COL * sizeof(state));

    }while (!sw.finished);

    /* Call this function if we exit() anywhere in the code */
    atexit(Neill_NCURS_Done);
    exit(EXIT_SUCCESS);

}


void change_elements(state oldState[ROW][COL], state newState[ROW][COL], char display[ROW][COL])
{
    int i, j;
    for (j = 0; j < ROW; j++){
        for (i = 0; i < ROW; i++){
            newState[j][i] = change_by_rules(oldState, j, i);
            display[j][i] = state_to_char(newState[j][i]);
        }
    }
    
}


state change_by_rules(state oldState[ROW][COL], int j, int i)
{
    state val;

    switch (oldState[j][i]){
        case empty:
            val = empty;
            break;
        case head:
            val = tail;
            break;
        case tail:
            val = copper;
            break;
        case copper:
            if (find_heads(oldState, j, i)){
                val = head;
            }
            else{
                val = copper;
            }
            break;
        default:
            val = initial;
    }
    return val;

}


int find_heads(state oldState[ROW][COL], int j, int i)
{
    int x, y, val, cnt = 0;

    for (y = j - 1; y <= j + 1; y++){
        for (x = i - 1; x <= i + 1; x++){
            /* When the cell is out of grid, ignore it */
            if ((x == i) && (y == j)){
                continue;
            } 
            else if ((x < 0) || (y < 0) || (x >= COL) || (y >= ROW)){
                continue;
            }
            else{
                if(oldState[y][x] == head){
                    cnt++;
                }
            }
        }
    }
    if((cnt == 1) || (cnt == 2)){
        val = 1;
    }
    else{
        val = 0;
    }
    return val;

}


state char_to_state(char c)
{
    state val;

    switch (c){
        case ' ': 
            val = empty;
            break;
        case 'H':
            val = head;
            break;
        case 't':
            val = tail;
            break;
        case 'c':
            val = copper;
            break;
        default:
            ErrInfo(INVALID_CHARACTER);
            exit(EXIT_FAILURE);
    }
    return val;

}


char state_to_char(state s)
{
    char val = ' ';

    switch (s){
        case empty:
            val = ' ';
            break;
        case head:
            val = 'H';
            break;
        case tail:
            val = 't';
            break;
        case copper:
            val = 'c';
            break;
        default:
            ErrInfo(INVALID_CHARACTER);
            exit(EXIT_FAILURE);
    }
    return val;

}


void ErrInfo(int ErrCode)
{
   if (ErrCode == INVALID_FILE){
       fprintf(stderr, "ERROR: Invalid file name.\n");
   }
   else if (ErrCode == INVALID_USAGE){
       fprintf(stderr, "ERROR: Invalid usage.\n");
   }
   else if (ErrCode == INVALID_CHARACTER){
       fprintf(stderr, "ERROR: Invalid characters in the file.\n");
   }

}


void test(void)
{
    int i, j;
    state a[ROW][COL], b[ROW][COL];
    int c[2][2], d[2][2];

    /* Test for function initialize_array() */
    for (j = 0; j < ROW; j++){
        for (i = 0; i < COL; i++){
            a[j][i] = initial;
            b[j][i] = initial;
        }
    }
    a[0][0] = empty, b[0][0] = empty;
    initialize_array(a, b);
    assert(a[0][0] == initial);
    assert(b[0][0] == initial);
    
    /* Test for file_to_array() */
    assert(fopen("abc.txt", "r") == NULL);
    assert(fopen("wireworld.txt", "r") != NULL);

    /* Test for char_to_state() */
    assert(char_to_state(' ') == empty);
    assert(char_to_state('H') == head);
    assert(char_to_state('t') == tail);
    assert(char_to_state('c') == copper);

    /* Test for change_by_rules() */
    a[0][0] = empty, a[0][1] = head, a[0][2] = tail;
    assert(change_by_rules(a, 0, 0) == empty);
    assert(change_by_rules(a, 0, 1) == tail);
    assert(change_by_rules(a, 0, 2) == copper);

    /* Test for find_heads() */
    b[0][0] = empty, b[0][1] = head, b[0][2] = tail;
    b[1][0] = copper, b[1][1] = copper, b[1][2] = head;
    b[2][0] = tail, b[2][1] = empty, b[2][2] = copper;
    
    assert(find_heads(b, 1, 0) == 1);
    assert(find_heads(b, 1, 1) == 1);
    assert(find_heads(b, 2, 2) == 1); 

    /* Test for state_to_char() */
    assert(state_to_char(empty) == ' ');
    assert(state_to_char(head) == 'H');
    assert(state_to_char(tail) == 't');
    assert(state_to_char(copper) == 'c');

    /* Test for memcpy() */
    for (j = 0; j < 2; j++){
        for (i = 0; i < 2; i++){
            c[j][i] = 1;
            d[j][i] = 2;
        }
    }
    memcpy(d, c, 2 * 2 * sizeof(int));
    for (j = 0; j < 2; j++){
        for (i = 0; i < 2; i++){
            assert(d[j][i] == 1);
        }
    }

}
