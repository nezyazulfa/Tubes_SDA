// include/dll.h
#ifndef DLL_H
#define DLL_H

#include "sll.h"

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data DNode (Doubly Linked List)
 * =====================================================================================
 * "Untuk fitur pengurutan berdasarkan tahun, kami mengonversi SLL menjadi
 * Doubly Linked List (DLL). Alasan utama penggunaan DLL adalah kemampuannya untuk
 * traversal dua arah (maju dan mundur) berkat adanya pointer 'prev' dan 'next'.
 * Hal ini membuat kami bisa menampilkan hasil urutan secara ascending (dari head)
 * dan descending (dari tail) dengan sangat mudah setelah proses sorting selesai."
 * =====================================================================================
 */
typedef struct DNode {
    Paper data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

DNode* convertSLLtoDLL(Node* sllHead);
void sortDLLByYear(DNode** headRef, int ascending);
void printDLL(DNode* head);
void printDLLBackward(DNode* tail);
void freeDLL(DNode* head);

#endif