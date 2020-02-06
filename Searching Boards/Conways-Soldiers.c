#include <stdio.h>
#include <stdlib.h>
#define WIDTH 7
#define HEIGHT 8

typedef struct node
{
    int array[HEIGHT][WIDTH];
    struct node *parent;
    struct node *next;
}Node;

typedef struct coordinate
{
    int x;
    int y;
}Coordinate;

Node *copy(Node *p);
void printToScreen(Node *p);
void printOut(Node *head, Node *p);
Node *copyBoard(Node *head, Node *p, int cnt);
Node *create(void);
Coordinate *createGoal(char *sx, char *sy);
void conwaysSoldiers(Node *head, Coordinate *goal);
int goalTest(Node *p, Coordinate *goal);
Node *nextStep(Node *head, Node *p);
int isVaild(Node *p, int x, int y);
int outboard(int x, int y);
Node *movgen(Node *head, Node *end, Node *p, int x, int y, int cnt);

int main(void)
{
    char *sx = "3";
    char *sy = "2";
    Node *head = create();
    Coordinate *goal = createGoal(sx, sy);

    conwaysSoldiers(head, goal);
    return 0;

}

void conwaysSoldiers(Node *head, Coordinate *goal)
{
    Node *p = head;
    while(!goalTest(p, goal)) {
        head = nextStep(head, p);
        p = p->next;
    }

    printOut(head, p);
}

void printOut(Node *head, Node *p)
{
    static int cnt = 0;
    if (p->parent != NULL) {
        printOut(head, p->parent);
    }
    printf("The No.%d step:\n", cnt);
    cnt++;
    printToScreen(p);
}

void printToScreen(Node *p)
{
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            printf("%d ", p->array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

Node *nextStep(Node *head, Node *p)
{
    Node *end = head;
    int i, j, cnt = 0;
    while (end->next != NULL) {
        end = end->next;
    }
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cnt = isVaild(p, i, j);
            if (cnt) {
                head = movgen(head, end, p, i, j, cnt);
                while (end->next != NULL) {
                    end = end->next;
                }
            }
        }
    }
    return head;
}

Node *movgen(Node *head, Node *end, Node *p, int x, int y, int cnt)
{
    int i, j;
    Node *temp = (Node *)malloc(sizeof(Node));
    Node *temphold = (Node *)malloc(sizeof(Node));
    temp = copyBoard(temp, p, cnt);
    temphold = temp;
    for (i = x - 1; i < x + 2; i++) {
        for (j = y - 1; j < y + 2; j++) {
            if (abs(i - x) + abs(j - y) == 1 && p->array[i][j] == 1) {
                if (outboard(2 * i - x, 2 * j - y)) {
                    if (p->array[abs(2 * i - x)][abs(2 * j - y)] == 0) {
                        temp->array[x][y] = 0;
                        temp->array[i][j] = 0;
                        temp->array[abs(2 * i - x)][abs(2 * j - y)] = 1;
                        temp = temp->next;
                    }
                }
            }
        }
    }
    end->next = temphold;
    return head;
}

Node *copyBoard(Node *head, Node *p, int cnt)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    Node *q = head;
    do{
        temp = copy(p);
        temp->parent = p;
        temp->next = NULL;
        q->next = temp;
        q = q->next;
        cnt--;
    }while (cnt > 0);

    return head->next;
}

Node *copy(Node *p)
{
    int i, j;
    Node *temp = (Node *)malloc(sizeof(Node));
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            temp->array[i][j] = p->array[i][j];
        }
    }
    return temp;
}

int isVaild(Node *p, int x, int y)
{
    int i, j, cnt = 0;
    if (p->array[x][y] == 0) {
        return 0;
    }
    for (i = x - 1; i < x + 2; i++) {
        for (j = y - 1; j < y + 2; j++) {
            if (abs(i - x) + abs(j - y) == 1 && p->array[i][j] == 1) {
                if(outboard(2 * i - x, 2 * j - y)) {
                    if(p->array[abs(2 * i - x)][abs(2 * j - y)] == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    return cnt;
}

int outboard(int x, int y)
{
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        return 0;
    }
    return 1;
}

int goalTest(Node *p, Coordinate *goal)
{
    if (p->array[goal->y][goal->x] == 1) {
        return 1;
    }
    return 0;
}

Coordinate *createGoal(char *sx, char *sy)
{
    Coordinate *goal = (Coordinate *)malloc(sizeof(Coordinate));
    goal->x = sx[0] - '0';
    goal->y = sy[0] - '0';
    return goal;
}

Node *create(void)
{
    int i, j;
    Node *head = (Node *)malloc(sizeof(Node));
    for (i = 0; i < HEIGHT / 2; i++) {
        for (j = 0; j < WIDTH; j++) {
            head->array[i][j] = 0;
            head->array[i + HEIGHT / 2][j] = 1;
        }
    }
    head->parent = NULL;
    head->next = NULL;
    return head;
}