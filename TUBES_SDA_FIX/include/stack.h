#ifndef STACK_H
#define STACK_H

#include "sll.h" // untuk typedef Paper

typedef struct StackNode {
    Paper data;
    struct StackNode* next;
} StackNode;

void push(StackNode** top, Paper data);
Paper pop(StackNode** top); // Sebaiknya periksa apakah stack kosong sebelum pop
void printBookmarks(StackNode* top);
void saveBookmarksToFile(StackNode* top, const char* filename);
void freeStack(StackNode* top);

#endif