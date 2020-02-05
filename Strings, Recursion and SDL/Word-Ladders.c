#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
    char *word;
    struct node *next;
}Node;

int compare(char *a, char *b);
int countDiff(char *a, char *b);
void wordLadders(char *start, char *end, Node *head, int length);
int notIn(char *temp, Node *head);
Node *create(char *start, char *end);
Node *insert(Node *head, char *word);
Node *del(Node *head, char *word);
void printLink(Node *head);
int linkCompare(Node *head);

int main(int argv, char **argc)
{
    if (argv == 3) {
    	char *start = argc[1], *end = argc[2];
        int diff = countDiff(start, end);
    	Node *head = create(start, end);
		wordLadders(start, end, head, diff);
	}
    else {
		printf("Error.\n");
	}

    return 0;
}

Node *create(char *start, char *end)
{
    Node *head = (Node *)malloc(sizeof(Node));
	Node *p = (Node *)malloc(sizeof(Node));
    head->word = (char *)malloc(sizeof(char) * strlen(start));
    head->word = start;
    p->word = (char *)malloc(sizeof(char) * strlen(end));
    p->word = end;
    head->next = p;
    p->next = NULL;
    
    return head;
}

Node *insert(Node *head, char *word)
{
    Node *p,*current = head;
    Node *new = (Node *)malloc(sizeof(Node));
    new->word = (char *)malloc(sizeof(char) * strlen(word));
    while (current->next->next != NULL) {
        current = current->next;
    }
    p = current->next;
    new->word = word;
    current->next = new;
    new->next = p;
    
    return head;
}

Node *del(Node *head, char *word)
{
    Node *p = head, *current = head;
    while (strcmp(p->word, word)) {
        current = p;
        p = p->next;
    }
    current->next = p->next;
    free(p);
    
    return head;
}

void printLink(Node *head)
{
    int i;
    Node *p = head;
    do {
        for(i = 0; i < strlen(p->word); i++) {
            printf("%c", p->word[i]);
        }
        if(p->next != NULL) {
            printf(" -> ");
        }
        p = p->next;
    }while (p != NULL);
    printf("\n");
}

int countDiff(char *a, char *b)
{
    int i, diff = 1;
    for(i = 0; i < strlen(a); i++) {
        if(*(a + i) != *(b + i)) {
            diff++;
        }
    }
    return diff;
}

void wordLadders(char *start, char *end, Node *head, int length)
{
    FILE *fp = fopen("34words.txt", "r");
    char *temp = (char *)malloc(sizeof(char) * strlen(start));
    if(linkCompare(head)) {
        printLink(head);
    }
    while(!(feof(fp)) && length > 1) {
        if(compare(start, temp) && notIn(temp,head) && length > 2) {
            head = insert(head, temp);
            wordLadders(temp, end, head, length - 1);
            head = del(head, temp);
        }
        fscanf(fp, "%s", temp);
    }

    fclose(fp);
}

int linkCompare(Node *head)
{
    Node *p = head;
    int diff = 0;
    if(p->next->next == NULL) {
        return 0;
    }
    while(p->next != NULL) {
        if(!compare(p->word, p->next->word)) {
            return 0;
        }
        p = p->next;
    }

    return 1;
}

int compare(char *a, char *b)
{
    int i, diff = 0;
    if (strlen(a) != strlen(b)) {
        return 0;
    }
    for (i = 0; i < strlen(a); i++) {
        if(*(a + i) != *(b + i)) {
            diff++;
            if(diff > 1) {
                return 0;
            }
        }
    }

    return (diff == 1) ? 1 : 0;
}

int notIn(char *temp, Node *head)
{
    Node *p = head;
    while (p != NULL) {
        if(!strcmp(p->word, temp)) {
            return 0;
        }
        p = p->next;
    }
    
    return 1;
}