#ifndef SLL_H
#define SLL_H

typedef struct Paper
{
    char id[1000];
    char title[1000];
    char incitation[1000];
    char author[1000];
    int year;
} Paper;

typedef struct Node
{
    Paper data;
    struct Node *next;
} Node;

Node *loadDataFromFile(const char *filename);
void printAllPapers(Node *head);
Node *searchPaperById(Node *head, const char *id);
void freeList(Node *head);

#endif
