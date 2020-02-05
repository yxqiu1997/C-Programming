#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int prime;
    struct node *next;
}Node;

Node *calPrime(char *argv);
Node *create(void);
Node *createPrime(int temp);
Node *insert(Node *head, int cnt);
Node *findPrime(Node *headprime, int temp);
void printLink(char *argv, Node *head);

int main(void)
{
    Node *head = calPrime("320");
    printLink("320", head);

    return 0;
}

Node *create(void)
{
    Node *head = (Node *)malloc(sizeof(Node));
    head->next = NULL;
    head->prime = 0;
    return head;
}    

Node *createPrime(int temp)
{
    int i, cnt = 2, flag = 0;
    Node *head = create();
    while (cnt <= temp) {
        for (i = 2; i<cnt; i++) {
            if (cnt % i == 0) {
                flag = 1;
            }
        }
        if (flag == 0 && i == cnt) {
            head = insert(head, cnt);
        }
        cnt++;
        flag = 0;
    }
    return head;
}

Node *insert(Node *head, int cnt)
{
    Node *p = head, *temp = create();
    if(head->next == NULL && head->prime == 0) {
        head->prime = cnt;
        return head;
    }
    while (p->next != NULL) {
        p = p->next;
    }
    temp->prime = cnt;
    p->next = temp;
    temp->next = NULL;

    return head;
}


Node *calPrime(char *argv)
{
    int temp = atoi(argv);
    Node *headprime = createPrime(temp);
    Node *p = headprime;
    Node *head = create();
    Node *restemp;
    Node *current = head;
    while(temp > 1) {
        restemp = create();
        p = findPrime(p, temp);
        restemp->prime = p->prime;
        current->next = restemp;
        current = current->next;
        temp = temp / p->prime;
    }
    return head->next;
}

Node *findPrime(Node *p, int temp)
{
    while (temp % p->prime != 0) {
        p = p->next;
    }
    return p;
}

void printLink(char *argv, Node *head)
{
    int cnt = 0, temp = 0;
    Node *p = head;  
    Node *headprime = createPrime(temp);
    printf("%d = ", atoi(argv));

    while (p != NULL) {
        temp = p->prime;
        do{
            cnt++;
            p = p->next;
            if (p == NULL) {
                break;
            }
        }while (temp == p->prime);
        printf("%d^%d ", temp, cnt);

        if(p != NULL) {
            printf("* ");
        }
        cnt = 0;
    }
}