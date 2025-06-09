// src/sll.c

/*
 * File: sll.c
 * Deskripsi: Implementasi semua operasi terkait Single Linked List (SLL).
 * SLL berfungsi sebagai struktur data dasar untuk memuat data dari file
 * sebelum diproses lebih lanjut ke dalam struktur data lain.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sll.h"

// Membuat node SLL baru dan menginisialisasi datanya.
Node* createNode(Paper data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode) {
        newNode->data = data;
        newNode->next = NULL;
    }
    return newNode;
}

// Membaca data dari file teks dan membangun SLL.
Node* loadDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Gagal membuka file");
        return NULL;
    }
    Node* head = NULL;
    Node* tail = NULL;
    Paper temp;
    while (fscanf(file, " %99[^|]|%999[^|]|%99[^|]|%999[^|]|%d\n",
                  temp.id, temp.title, temp.incitation, temp.author, &temp.year) == 5) {
        Node* newNode = createNode(temp);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);
    return head;
}

// Helper untuk memotong dan mencetak string yang terlalu panjang.
void print_truncated(const char* text, int max_len) {
    if (strlen(text) > max_len) {
        printf("%.*s...", max_len - 3, text);
    } else {
        printf("%s", text);
    }
}

// Menampilkan semua paper dalam format kartu yang rapi.
void printAllPapers(Node* head) {
    if (!head) {
        printf("\n  [INFO] Tidak ada paper untuk ditampilkan.\n");
        return;
    }
    Node* curr = head;
    int count = 1;
    while (curr) {
        printf("+--[ Paper #%d ]---------------------------------------------------------------+\n", count++);
        printf("| ID    : %s\n", curr->data.id);
        printf("| Judul : ");
        print_truncated(curr->data.title, 65);
        printf("\n");
        printf("| Tahun : %d\n", curr->data.year);
        printf("+------------------------------------------------------------------------------+\n\n");
        curr = curr->next;
    }
}

// Melakukan pencarian linear O(n) pada SLL.
Node* searchPaperById(Node* head, const char* id) {
    Node* current = head;
    while (current) {
        if (strcmp(current->data.id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Membebaskan seluruh memori yang dialokasikan untuk SLL.
void freeList(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}