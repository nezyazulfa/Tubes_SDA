#include "include/dll.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DoubleLinkedList* createDoubleList() {
    DoubleLinkedList* list = (DoubleLinkedList*)malloc(sizeof(DoubleLinkedList));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
        list->count = 0;
    }
    return list;
}

void insertDouble(DoubleLinkedList* list, Paper paper) { // Insert at tail
    if (!list) return;
    NodeDouble* newNode = (NodeDouble*)malloc(sizeof(NodeDouble));
    if (!newNode) return;
    newNode->data = paper;
    newNode->next = NULL;
    newNode->prev = list->tail;
    if (list->tail == NULL) { // List is empty
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->count++;
}

void displayDoubleForward(DoubleLinkedList* list) {
    if (!list) return;
    printf("\n=== DOUBLE LINKED LIST (Forward Traversal) ===\n");
    printf("+------+------------------------------------------+------------+-------+\n");
    printf("| No.  | Title                                    | Citations  | Year  |\n");
    printf("+------+------------------------------------------+------------+-------+\n");
    NodeDouble* current = list->head;
    int index = 1;
    while (current != NULL && index <= 10) { // Batasi output
        char shortTitle[40];
        strncpy(shortTitle, current->data.title, 37);
        shortTitle[37] = '\0';
        if (strlen(current->data.title) > 37) strcat(shortTitle, "...");
        printf("| %-4d | %-35s | %-10d | %-5d |\n", index, shortTitle, current->data.inCitations, current->data.year);
        current = current->next;
        index++;
    }
    printf("+------+------------------------------------------+------------+-------+\n");
    printf("Total papers in DLL: %d (displaying up to 10 forward)\n", list->count);
}

void displayDoubleBackward(DoubleLinkedList* list) {
    if (!list) return;
    printf("\n=== DOUBLE LINKED LIST (Backward Traversal) ===\n");
    printf("+------+------------------------------------------+------------+-------+\n");
    printf("| No.  | Title                                    | Citations  | Year  |\n");
    printf("+------+------------------------------------------+------------+-------+\n");
    NodeDouble* current = list->tail;
    int index = 1;
    while (current != NULL && index <= 10) { // Batasi output
        char shortTitle[40];
        strncpy(shortTitle, current->data.title, 37);
        shortTitle[37] = '\0';
        if (strlen(current->data.title) > 37) strcat(shortTitle, "...");
        printf("| %-4d | %-35s | %-10d | %-5d |\n", index, shortTitle, current->data.inCitations, current->data.year);
        current = current->prev;
        index++;
    }
    printf("+------+------------------------------------------+------------+-------+\n");
    printf("Total papers in DLL: %d (displaying up to 10 backward)\n", list->count);
}

void freeDoubleList(DoubleLinkedList* list) {
    if (!list) return;
    NodeDouble* current = list->head;
    NodeDouble* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    free(list);
}