// src/dll.c

/*
 * File: dll.c
 * Deskripsi: Implementasi logika Doubly Linked List (DLL).
 * Digunakan secara spesifik untuk fitur pengurutan paper berdasarkan tahun.
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/dll.h"

// Membuat node DLL baru.
DNode* createDNode(Paper data) {
    DNode* newNode = (DNode*)malloc(sizeof(DNode));
    if (newNode) {
        newNode->data = data;
        newNode->prev = newNode->next = NULL;
    }
    return newNode;
}

// Mengonversi SLL menjadi DLL sebagai persiapan untuk sorting.
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

// Mengurutkan DLL menggunakan Bubble Sort (O(n^2)).
void sortDLLByYear(DNode** headRef, int ascending) {
    if (!headRef || !(*headRef)) return;
    int swapped;
    DNode* ptr;
    DNode* lptr = NULL;
    do {
        swapped = 0;
        ptr = *headRef;
        while (ptr->next != lptr) {
            int condition = ascending ? (ptr->data.year > ptr->next->data.year)
                                      : (ptr->data.year < ptr->next->data.year);
            if (condition) {
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

// Helper untuk mencetak detail satu paper dari DLL.
void printPaperDetailsDLL(Paper data, int* count) {
    printf("\n  ---[ No. %d | Tahun: %d ]---\n", *count, data.year);
    printf("    ID    : %s\n", data.id);
    printf("    Judul : ");
    print_truncated(data.title, 60);
    printf("\n");
    (*count)++;
}

// Menampilkan DLL secara ascending.
void printDLL(DNode* head) {
    if (!head) {
        printf("\n  [INFO] List kosong untuk ditampilkan.\n");
        return;
    }
    DNode* current = head;
    int paperCount = 1;
    while (current) {
        printPaperDetailsDLL(current->data, &paperCount);
        current = current->next;
    }
}

// Menampilkan DLL secara descending.
void printDLLBackward(DNode* tail) {
    if (!tail) {
        printf("\n  [INFO] List kosong untuk ditampilkan.\n");
        return;
    }
    DNode* current = tail;
    int paperCount = 1;
    while (current) {
        printPaperDetailsDLL(current->data, &paperCount);
        current = current->prev;
    }
}

// Membebaskan memori DLL.
void freeDLL(DNode* head) {
    DNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}