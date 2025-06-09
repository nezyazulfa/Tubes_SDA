// include/stack.h

#ifndef STACK_H
#define STACK_H

#include "sll.h"

/*
 * =====================================================================================
 * Penjelasan Struktur Data Stack
 * =====================================================================================
 * Stack (Tumpukan) adalah struktur data yang menganut prinsip LIFO (Last-In, First-Out).
 * Data yang terakhir masuk adalah data yang pertama keluar.
 * - 'top': Pointer yang menunjuk ke elemen paling atas dari tumpukan.
 *
 * Mengapa Stack? Stack sangat intuitif untuk fitur "Bookmark". Ketika pengguna
 * menambahkan bookmark baru, bookmark tersebut diletakkan di atas tumpukan.
 * Saat pengguna melihat daftar bookmark, yang paling relevan (yang baru
 * ditambahkan) akan muncul pertama kali. Ini sama seperti tumpukan buku di meja.
 * =====================================================================================
 */
typedef struct StackNode {
    Paper data;
    struct StackNode* next;
} StackNode;

// Menambah elemen ke atas stack.
void push(StackNode** top, Paper data);

// Mengambil elemen dari atas stack.
Paper pop(StackNode** top);

// Menampilkan daftar bookmark.
void printBookmarks(StackNode* top);

// Menyimpan bookmark ke file.
void saveBookmarksToFile(StackNode* top, const char* filename);

// Membebaskan memori stack.
void freeStack(StackNode* top);

#endif