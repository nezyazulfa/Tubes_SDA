// src/sll.c

/*
 * File ini bertanggung jawab untuk semua operasi terkait Single Linked List (SLL).
 * Termasuk memuat data dari file, mencari data, menampilkan, dan membebaskan memori.
 * SLL menjadi fondasi untuk struktur data lain seperti Tree dan DLL.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sll.h"

// Fungsi internal untuk membuat node SLL baru.
Node* createNode(Paper data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode) {
        newNode->data = data;
        newNode->next = NULL;
    }
    return newNode;
}

/*
 * =====================================================================================
 * Penjelasan Fungsi loadDataFromFile
 * =====================================================================================
 * Fungsi ini membaca file 'data.txt' baris per baris dan membangun sebuah SLL.
 *
 * Algoritma:
 * 1. Buka file dalam mode baca ('r').
 * 2. Gunakan loop `while` dengan `fscanf` untuk membaca setiap baris sampai akhir file (EOF).
 * 3. Format string `"%99[^|]|%999[^|]|%99[^|]|%999[^|]|%d\n"` adalah kunci utama:
 * - `%99[^|]`: Baca hingga 99 karakter ATAU sampai bertemu karakter `|`.
 * Karakter `|` tidak ikut dibaca. Ini mencegah buffer overflow.
 * - `|`: Setelah membaca string, `fscanf` akan mencocokkan dan melewati karakter `|`.
 * 4. Setiap baris yang berhasil dibaca (return value `fscanf` == 5) diubah menjadi `Node` baru.
 * 5. Node baru ditambahkan ke akhir SLL untuk menjaga urutan asli dari file.
 * =====================================================================================
 */
Node* loadDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Gagal membuka file");
        return NULL;
    }

    Node* head = NULL;
    Node* tail = NULL; // Pointer ke node terakhir untuk efisiensi O(1) saat insert
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

/*
 * =====================================================================================
 * Penjelasan Fungsi print_truncated
 * =====================================================================================
 * Fungsi helper ini digunakan di seluruh program untuk menjaga tampilan UI tetap rapi.
 * Jika sebuah teks (seperti judul) lebih panjang dari batas `max_len`,
 * fungsi ini akan memotongnya dan menambahkan "..." di akhir.
 * `%.*s`: Ini adalah format specifier di C yang memungkinkan kita menentukan
 * panjang maksimum string yang akan dicetak secara dinamis.
 * =====================================================================================
 */
void print_truncated(const char* text, int max_len) {
    if (strlen(text) > max_len) {
        printf("%.*s...", max_len - 3, text);
    } else {
        printf("%s", text);
    }
}

// Menampilkan semua paper dengan format yang rapi menggunakan print_truncated.
void printAllPapers(Node* head) {
    if (!head) {
        printf("\n  [INFO] Tidak ada paper untuk ditampilkan.\n");
        return;
    }
    printf("\n  +----------------------------------------------------+\n");
    printf("  |           DAFTAR SEMUA PAPER (URUTAN ASLI)       |\n");
    printf("  +----------------------------------------------------+\n");

    Node* curr = head;
    while (curr) {
        printf("\n  ---[ Paper ID: %s ]---\n", curr->data.id);
        printf("    Judul        : ");
        print_truncated(curr->data.title, 60);
        printf("\n");
        printf("    Mensitasi ID : %s\n", curr->data.incitation);
        printf("    Author       : ");
        print_truncated(curr->data.author, 60);
        printf("\n");
        printf("    Tahun        : %d\n", curr->data.year);
        curr = curr->next;
    }
    printf("\n  -- Akhir Daftar --\n");
}

/*
 * =====================================================================================
 * Penjelasan Fungsi searchPaperById
 * =====================================================================================
 * Fungsi ini melakukan pencarian linear (linear search) pada SLL.
 * Ia melintasi (traverse) setiap node dari 'head' sampai 'tail'.
 * Di setiap node, ia membandingkan ID yang dicari dengan ID paper di node tersebut
 * menggunakan `strcmp()`. `strcmp()` mengembalikan 0 jika kedua string sama.
 * Jika ditemukan, ia mengembalikan pointer ke `Node` tersebut. Jika tidak, NULL.
 * Kompleksitas waktu: O(n), di mana n adalah jumlah node.
 * Fungsi ini masih dipertahankan untuk fitur seperti "Tambah Bookmark" di mana
 * kita perlu node asli dari SLL.
 * =====================================================================================
 */
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

// Membebaskan setiap node dalam SLL untuk mencegah memory leak.
void freeList(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}