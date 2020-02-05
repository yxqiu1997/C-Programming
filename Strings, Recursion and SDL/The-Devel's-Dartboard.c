#include <stdio.h>
#include <stdlib.h>
#define SIZE 20
#define REPEATTIME 5000000

typedef struct Node
{
    int num;
    struct Node *next;
}Node;

Node *create(void);
void changeValue(Node *before, Node *after);
int countSum(Node *head);
void printOut(int max, Node *head);

int main(void)
{
    Node *head = create();
    Node *temp = head->next;
    int i, j, min, test;
    srand((unsigned)time(NULL));
    min = countSum(head);
    for (i = 0; i < REPEATTIME; i++) {
        changeValue(head, temp);
        test = countSum(head);
        if(min >= test) {
            for (j = 0; j < rand() % 20; j++) {
                temp = temp->next;
            }
            head = head->next;
            min = test;
        }
        else {
            changeValue(head, temp);
            for(j = 0; j < rand() % 20; j++) {
                temp = temp->next;
            }
            head=head->next;
        }
    }
    printOut(min, head);
}

Node *create(void)
{
    Node *head, *p, *temp;
    head = (Node *)malloc(sizeof(Node));
    p = head;
    p->next = NULL;
    p->num = 1;
    int i;
    for (i = 2; i <= SIZE; i++) {
        temp = (Node *)malloc(sizeof(Node));
        temp->num = i;
        p->next = temp;
        p = temp;
    }
    p->next = head;

    return head;
}

void changeValue(Node *before, Node *after)
{
    int temp;
    temp = before->num;
    before->num = after->num;
    after->num = temp;
}

int countSum(Node *head)
{
    Node *p = head;
    int i, sum = 0;
    for (i = 0; i < SIZE; i++) {
        sum += (p->num + p->next->num + p->next->next->num) *\
            (p->num + p->next->num + p->next->next->num);
        p = p->next;
    }

    return sum;
}

void printOut(int max, Node *head)
{
    int i;
    printf("Total = %d :", max);
    for (i = 0; i < SIZE ; i++) {
        printf("%d ", head->num);
        head = head->next;
    }
    printf("\n");
}