// include/stack.h
#ifndef STACK_H
#define STACK_H

#include "sll.h"

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data Stack
 * =====================================================================================
 * "Fitur bookmark pada program kami diimplementasikan menggunakan Stack (Tumpukan).
 * Stack menganut prinsip LIFO (Last-In, First-Out). Pilihan ini sangat intuitif
 * dari sudut pandang pengguna. Ketika pengguna mem-bookmark sebuah paper, paper
 * tersebut diletakkan di 'puncak' tumpukan. Saat daftar bookmark ditampilkan,
 * paper yang terakhir ditambahkan (yang paling mungkin masih relevan) akan
 * muncul pertama kali, sama seperti tumpukan buku di meja."
 * =====================================================================================
 */
typedef struct StackNode {
    Paper data;
    struct StackNode* next;
} StackNode;

void push(StackNode** top, Paper data);
Paper pop(StackNode** top);
void printBookmarks(StackNode* top);
void saveBookmarksToFile(StackNode* top, const char* filename);
void freeStack(StackNode* top);

#endif