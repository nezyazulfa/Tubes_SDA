#include <stdio.h>
#include <stdlib.h>
#include "../include/dll.h"

DNode* createDNode(Paper data) {
    DNode* newNode = (DNode*) malloc(sizeof(DNode));
    if (newNode) {
        newNode->data = data;
        newNode->prev = newNode->next = NULL;
    }
    return newNode;
}

DNode* convertSLLtoDLL(Node* sllHead) {
    DNode* dllHead = NULL;
    DNode* dllTail = NULL;

    while (sllHead) {
        DNode* newNode = createDNode(sllHead->data);
        if (!dllHead) {
            dllHead = dllTail = newNode;
        } else {
            dllTail->next = newNode;
            newNode->prev = dllTail;
            dllTail = newNode;
        }
        sllHead = sllHead->next;
    }
    return dllHead;
}

void sortDLLByYear(DNode** headRef, int terkecil_terbesar) {
    if (!headRef || !(*headRef)) return;

    int swapped;
    DNode* ptr;
    DNode* lptr = NULL;

    do {
        swapped = 0;
        ptr = *headRef;

        while (ptr->next != lptr) {
            int compare = terkecil_terbesar ? (ptr->data.year > ptr->next->data.year) : (ptr->data.year < ptr->next->data.year);
            if (compare) {
                Paper temp = ptr->data;
                ptr->data = ptr->next->data;
                ptr->next->data = temp;
                swapped = 1;
            }
            ptr = ptr->next;
        }
        lptr = ptr;
    } while (swapped);
}

// Helper di dll.c (versi ASCII)
void printPaperDetailsDLL(Paper data, int* count) {
    if (count) {
        printf("\n  --- Paper %d ---\n", (*count)++);
    } else {
        printf("\n  ---------------\n");
    }
    printf("    ID           : %s\n", data.id);
    printf("    Judul        : %s\n", data.title);
    printf("    Mensitasi ID : %s\n", data.incitation);
    printf("    Author       : %s\n", data.author);
    printf("    Tahun        : %d\n", data.year);
}

void printDLL(DNode* head) {
    if (!head) {
        printf("\n  [INFO] List DLL kosong untuk ditampilkan (Urutan Ascending).\n");
        return;
    }
    printf("\n  +----------------------------------------------------+\n");
    printf("  |      DAFTAR PAPER (URUT TAHUN ASCENDING)         |\n");
    printf("  +----------------------------------------------------+\n");
    DNode* current = head;
    int paperCount = 1;
    while (current) {
        printPaperDetailsDLL(current->data, &paperCount);
        current = current->next;
    }
    printf("\n  -- Akhir Daftar (Ascending) --\n");
}

void printDLLBackward(DNode* tail) {
    if (!tail) {
        printf("\n  [INFO] List DLL kosong untuk ditampilkan (Urutan Descending).\n");
        return;
    }
    printf("\n  +----------------------------------------------------+\n");
    printf("  |     DAFTAR PAPER (URUT TAHUN DESCENDING)         |\n");
    printf("  +----------------------------------------------------+\n");
    DNode* current = tail;
    int paperCount = 1;
    while (current) {
        printPaperDetailsDLL(current->data, &paperCount);
        current = current->prev;
    }
    printf("\n  -- Akhir Daftar (Descending) --\n");
}

void freeDLL(DNode* head) {
    DNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
