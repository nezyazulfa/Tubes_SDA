#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sll.h"

Node *createNode(Paper data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode)
    {
        newNode->data = data;
        newNode->next = NULL;
    }
    return newNode;
}

Node *loadDataFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Gagal membuka file");
        return NULL;
    }

    Node *head = NULL;
    Node *tail = NULL;
    Paper temp;

    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  temp.id, temp.title, temp.incitation, temp.author, &temp.year) == 5)
    {
        Node *newNode = createNode(temp);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(file);
    return head;
}

void printAllPapers(Node *head)
{
    Node *curr = head;
    if (!curr)
    {
        printf("\n  [INFO] Tidak ada paper untuk ditampilkan.\n");
        return;
    }
    printf("\n  +----------------------------------------------------+\n");
    printf("  |           DAFTAR SEMUA PAPER (ASLI)            |\n");
    printf("  +----------------------------------------------------+\n");
    int count = 1;
    while (curr)
    {
        printf("\n  --- Paper %d ---\n", count++);
        printf("    ID           : %s\n", curr->data.id);
        printf("    Judul        : %s\n", curr->data.title);
        printf("    Mensitasi ID : %s\n", curr->data.incitation);
        printf("    Author       : %s\n", curr->data.author);
        printf("    Tahun        : %d\n", curr->data.year);
        curr = curr->next;
    }
    printf("\n  -- Akhir Daftar --\n");
}

Node *searchPaperById(Node *head, const char *id)
{
    Node *dhanu = head;
    while (dhanu)
    {
        if (strcmp(dhanu->data.id, id) == 0)
        {
            return dhanu;
        }
        dhanu = dhanu->next;
    }
    return NULL;
}

void freeList(Node *head)
{
    Node *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
    if (head == NULL)
    {
        printf("List telah dibebaskan.\n");
    }
    else
    {
        printf("Gagal membebaskan list.\n");
    }
}
