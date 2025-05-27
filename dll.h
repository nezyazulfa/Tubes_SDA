#ifndef DLL_H
#define DLL_H

#include "paper_data.h"
#include <stdio.h>

typedef struct NodeDouble {
    Paper data;
    struct NodeDouble* next;
    struct NodeDouble* prev;
} NodeDouble;

typedef struct DoubleLinkedList {
    NodeDouble* head;
    NodeDouble* tail;
    int count;
} DoubleLinkedList;

DoubleLinkedList* createDoubleList();
void insertDouble(DoubleLinkedList* list, Paper paper); // Insert at tail
void displayDoubleForward(DoubleLinkedList* list);
void displayDoubleBackward(DoubleLinkedList* list);
void freeDoubleList(DoubleLinkedList* list); // Fungsi untuk membebaskan memori


#endif // DLL_H