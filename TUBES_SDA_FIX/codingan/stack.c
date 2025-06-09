// src/stack.c

/*
 * File: stack.c
 * Deskripsi: Implementasi struktur data Stack (Tumpukan) dengan prinsip LIFO.
 * Digunakan untuk fitur bookmark.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"

void push(StackNode** top, Paper data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return;
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

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

void printBookmarks(StackNode* top) {
    if (!top) {
        printf("\n  [INFO] Tidak ada bookmark untuk ditampilkan.\n");
        return;
    }
    StackNode* current = top;
    while (current) {
        printf("\n  ---[ Bookmark: %s ]---\n", current->data.id);
        printf("      Judul: ");
        print_truncated(current->data.title, 60);
        printf("\n      Tahun: %d\n", current->data.year);
        current = current->next;
    }
}

void saveBookmarksToFile(StackNode* top, const char* filename) {
    // PENGECEKAN BARU: Pastikan stack tidak kosong sebelum melakukan apapun.
    if (top == NULL) {
        printf("\n  [INFO] Daftar bookmark kosong. Tidak ada yang disimpan.\n");
        return; // Keluar dari fungsi jika tidak ada data
    }

    // Kode di bawah ini hanya akan berjalan jika bookmark ada.
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("  [ERROR] Gagal membuka file untuk menyimpan bookmark");
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

void freeStack(StackNode* top) {
    while (top) {
        StackNode* temp = top;
        top = top->next;
        free(temp);
    }
}