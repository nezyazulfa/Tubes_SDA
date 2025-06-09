// src/stack.c

/*
 * File ini berisi implementasi struktur data Stack (Tumpukan) - LIFO.
 * Digunakan untuk fitur bookmark, di mana bookmark terakhir yang ditambahkan
 * akan menjadi yang pertama ditampilkan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"

// Menambahkan data ke atas tumpukan.
void push(StackNode** top, Paper data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return;
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

// Mengambil data dari atas tumpukan.
Paper pop(StackNode** top) {
    Paper emptyPaper = {"-1", "Stack Kosong", "N/A", "N/A", 0};
    if (!*top) {
        return emptyPaper;
    }
    StackNode* temp = *top;
    Paper data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

// Menampilkan semua bookmark dari atas ke bawah.
void printBookmarks(StackNode* top) {
    if (!top) {
        printf("\n  [INFO] Tidak ada bookmark untuk ditampilkan.\n");
        return;
    }
    printf("\n  +----------------------------------------------------+\n");
    printf("  |        DAFTAR BOOKMARK (Terbaru di Atas)         |\n");
    printf("  +----------------------------------------------------+\n");
    StackNode* current = top;
    while (current) {
        printf("\n  --- Bookmark ID: %s ---\n", current->data.id);
        printf("      Judul: ");
        print_truncated(current->data.title, 60);
        printf("\n      Tahun: %d\n", current->data.year);
        current = current->next;
    }
    printf("\n  -- Akhir Daftar Bookmark --\n");
}

/*
 * =====================================================================================
 * Penjelasan saveBookmarksToFile
 * =====================================================================================
 * Menyimpan data dari stack ke file teks. Karena kita men-traverse stack dari
 * 'top' ke bawah, urutan di file akan sama dengan yang ditampilkan di layar
 * (LIFO - yang terbaru di baris paling atas file).
 * Format penyimpanan `|` (pipe) dipilih agar konsisten dengan file input.
 * =====================================================================================
 */
void saveBookmarksToFile(StackNode* top, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("Gagal membuka file untuk menyimpan bookmark");
        return;
    }
    StackNode* current = top;
    while (current) {
        fprintf(fp, "%s|%s|%s|%s|%d\n",
                current->data.id,
                current->data.title,
                current->data.incitation,
                current->data.author,
                current->data.year);
        current = current->next;
    }
    fclose(fp);
    printf("\n  [OK] Bookmark berhasil disimpan ke %s\n", filename);
}

// Membebaskan semua node di dalam stack.
void freeStack(StackNode* top) {
    while (top) {
        StackNode* temp = top;
        top = top->next;
        free(temp);
    }
}