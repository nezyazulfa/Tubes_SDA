#ifndef DLL_H
#define DLL_H

#include "sll.h"  // untuk typedef Paper

typedef struct DNode {
    Paper data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

DNode* convertSLLtoDLL(Node* sllHead);
void sortDLLByYear(DNode** headRef, int ascending);
void printPaperDetailsDLL(Paper data, int* count);
void printDLL(DNode* head);
void printDLLBackward(DNode* tail);
void freeDLL(DNode* head);

#endif
