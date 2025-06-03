#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h" // Sesuaikan path

void push(StackNode** top, Paper data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        perror("Gagal alokasi memori untuk StackNode");
        return;
    }
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

Paper pop(StackNode** top) {
    Paper emptyPaper = {"-1", "Stack Kosong", "N/A", "N/A", 0}; // Data sentinel
    if (!*top) {
        // printf("Stack kosong, tidak bisa pop.\n");
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
    printf("\n  +----------------------------------------------------+\n");
    printf("  |        DAFTAR BOOKMARK (Terbaru di Atas)         |\n");
    printf("  +----------------------------------------------------+\n");
    StackNode* current = top;
    int count = 1;
    while (current) {
        printf("\n  [%d] ID    : %s\n", count++, current->data.id);
        printf("      Judul : %s\n", current->data.title);
        printf("      Tahun : %d\n", current->data.year);
        current = current->next;
    }
    printf("\n  -- Akhir Daftar Bookmark --\n");
}

void saveBookmarksToFile(StackNode* top, const char* filename) {
    FILE* fp = fopen(filename, "w"); // "w" untuk overwrite, "a" untuk append
    if (!fp) {
        perror("Gagal membuka file untuk menyimpan bookmark");
        return;
    }
    // Untuk menyimpan dalam urutan LIFO (seperti di stack), kita perlu reverse
    // atau pop satu per satu dan tulis. Cara mudah adalah traverse dan tulis,
    // ini akan menyimpan dalam urutan terbalik dari stack (yang tertua dulu).
    // Jika ingin urutan sama seperti saat printBookmarks (terbaru dulu),
    // kita harus pop dan simpan ke file, atau simpan ke temp SLL lalu tulis.
    // Untuk kesederhanaan, kita tulis sesuai urutan traversal dari top.
    StackNode* current = top;
    while (current) {
        fprintf(fp, "%s|%s|%s|%s|%d\n",
                current->data.id, current->data.title,
                current->data.incitation, current->data.author,
                current->data.year);
        current = current->next;
    }
    fclose(fp);
    printf("Bookmark berhasil disimpan ke %s.\n", filename);
}

void freeStack(StackNode* top) {
    while (top) {
        StackNode* temp = top;
        top = top->next;
        free(temp);
    }
}