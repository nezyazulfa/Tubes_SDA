// include/dll.h

#ifndef DLL_H
#define DLL_H

#include "sll.h"

/*
 * =====================================================================================
 * Penjelasan Struktur Data DNode (Doubly Linked List)
 * =====================================================================================
 * Struct 'DNode' adalah komponen pembangun Doubly Linked List (DLL).
 * - 'prev': Pointer yang menunjuk ke node SEBELUMNYA.
 * - 'next': Pointer yang menunjuk ke node BERIKUTNYA.
 *
 * Mengapa DLL? DLL digunakan untuk fitur pengurutan berdasarkan tahun.
 * Keunggulan utamanya adalah kemampuan traversal dua arah (maju dan mundur).
 * Ini memungkinkan kita untuk menampilkan hasil urutan secara ascending (dari head)
 * dan descending (dari tail) dengan sangat mudah dan efisien.
 * =====================================================================================
 */
typedef struct DNode {
    Paper data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

// Konversi SLL ke DLL untuk pengurutan.
DNode* convertSLLtoDLL(Node* sllHead);

// Mengurutkan DLL berdasarkan tahun.
void sortDLLByYear(DNode** headRef, int ascending);

// Menampilkan daftar dari depan (ascending).
void printDLL(DNode* head);

// Menampilkan daftar dari belakang (descending).
void printDLLBackward(DNode* tail);

// Membebaskan memori DLL.
void freeDLL(DNode* head);

#endif