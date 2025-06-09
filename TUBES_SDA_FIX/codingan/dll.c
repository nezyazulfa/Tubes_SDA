// src/dll.c

/*
 * File ini mengimplementasikan logika Doubly Linked List (DLL).
 * Digunakan secara spesifik untuk fitur pengurutan paper berdasarkan tahun,
 * karena DLL memudahkan traversal maju (ascending) dan mundur (descending).
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/dll.h"

// Fungsi internal untuk membuat DNode baru.
DNode* createDNode(Paper data) {
    DNode* newNode = (DNode*)malloc(sizeof(DNode));
    if (newNode) {
        newNode->data = data;
        newNode->prev = newNode->next = NULL;
    }
    return newNode;
}

// Mengonversi seluruh SLL menjadi DLL.
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

/*
 * =====================================================================================
 * Penjelasan Fungsi sortDLLByYear
 * =====================================================================================
 * Fungsi ini mengurutkan DLL menggunakan algoritma Bubble Sort.
 *
 * Mengapa Bubble Sort? Bubble Sort sengaja dipilih karena implementasinya yang
 * paling sederhana dan mudah dipahami, sangat cocok untuk tujuan pembelajaran
 * dan demonstrasi struktur data dasar. Ia bekerja dengan cara berulang kali
 * menukar elemen yang bersebelahan jika urutannya salah.
 *
 * Kompleksitas Waktu: O(n^2), yang tidak efisien untuk dataset besar.
 * Untuk aplikasi nyata dengan data besar, algoritma seperti Merge Sort (O(n log n))
 * akan jauh lebih superior karena kecepatannya. Kemampuan untuk menjelaskan
 * trade-off antara kesederhanaan implementasi dan efisiensi ini adalah poin penting.
 * =====================================================================================
 */
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

// Fungsi helper untuk mencetak detail satu paper, agar tidak duplikasi kode.
void printPaperDetailsDLL(Paper data, int* count) {
    printf("\n  ---[ No. %d | Tahun: %d ]---\n", *count, data.year);
    printf("    ID    : %s\n", data.id);
    printf("    Judul : ");
    print_truncated(data.title, 60);
    printf("\n");
    (*count)++;
}

// Menampilkan DLL dari head ke tail (Ascending).
void printDLL(DNode* head) {
    if (!head) {
        printf("\n  [INFO] List kosong untuk ditampilkan.\n");
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

// Menampilkan DLL dari tail ke head (Descending).
void printDLLBackward(DNode* tail) {
    if (!tail) {
        printf("\n  [INFO] List kosong untuk ditampilkan.\n");
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

// Membebaskan memori DLL.
void freeDLL(DNode* head) {
    DNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}