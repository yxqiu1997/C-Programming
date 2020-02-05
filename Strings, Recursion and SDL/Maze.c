#include <stdio.h>
#include <stdlib.h>

#define N 10
#define entrance_x 0
#define entrance_y 1
#define valid_route ' '
#define already_pass '1' 
#define fault 'X'

struct Position{
    int x;
    int y;
};
typedef struct Position Position;

void read_maze(char maze[N][N], char *filename);
int explore(Position entry, Position current, char maze[N][N]);
int isValid_route(Position current, char maze[N][N]);
int isExit(Position entry, Position current);
void printMaze(char maze[N][N]);

int main(int argc, char **argv)
{
    char maze[N][N];
    int exist = 0;
    Position current, entry;
    entry.x = entrance_x, entry.y = entrance_y;
    current.x = entry.x, current.y = entry.y;

    if (argc == 2){
        read_maze(maze, argv[1]);
        exist = explore(entry, current, maze);
        printMaze(maze);
        if (exist){
            printf("Route Exists.\n");
        }
        else{
            printf("No Route Exists.\n");
        }
    }
    else{
        fprintf(stderr, "ERROR: Invalid Usage.\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

void read_maze(char maze[N][N], char *filename)
{
    FILE *fp;
    int i, j;
    char c;

    if ((fp = fopen(filename, "r")) == NULL){
        fprintf(stderr, "ERROR: Invalid Filename.\n");
        exit(EXIT_FAILURE);
    }
    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            c = getc(fp);
            if (c != '\n'){
                maze[j][i] = c;
            }
            else{
                i--;
            }
        }
    }
}

int explore(Position entry, Position current, char maze[N][N])
{
    Position next;
    if (isValid_route(current, maze)){

        maze[current.y][current.x] = already_pass;

        if(isExit(entry, current)){
            return 1;
        }

        next = current;
        next.y++;
        if(explore(entry, next, maze)){
            return 1;
        }

        next = current;
        next.x--;
        if(explore(entry, next, maze)){
            return 1;
        }

        next = current;
        next.x++;
        if(explore(entry, next, maze)){
            return 1;
        }
    
        next = current;
        next.y--;
        if(explore(entry, next, maze)){
            return 1;
        }

        maze[current.y][current.x] = fault;
    }
    return 0;
}

int isValid_route(Position current, char maze[N][N])
{
    if (maze[current.y][current.x] == ' '){
        return 1;
    }
    return 0;
}

int isExit(Position entry, Position current)
{
    if (current.x == entry.x && current.y == entry.y){
        return 0;
    }
    if (current.x == 0 || current.x == N - 1 || current.y == 0 || current.y == N - 1){
        return 1;
    }
    return 0;
}

void printMaze(char maze[N][N])
{
    int i, j;
    for (j = 0; j < N; j++){
        for (i = 0; i < N; i++){
            printf("%c", maze[j][i]);
        }
        printf("\n");
    }
}