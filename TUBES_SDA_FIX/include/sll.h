// include/sll.h
#ifndef SLL_H
#define SLL_H

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data Paper
 * =====================================================================================
 * "Struct 'Paper' ini, Pak/Bu, adalah representasi data fundamental dalam program kami.
 * Setiap objek 'Paper' menyimpan semua informasi esensial dari satu karya ilmiah.
 * Kami menggunakan char array dengan ukuran tetap seperti id[100] untuk simplisitas
 * dalam manajemen memori pada skala proyek ini. Kami sadar bahwa untuk data yang
 * ukurannya sangat bervariasi, alokasi memori dinamis bisa lebih efisien,
 * namun pendekatan ini sudah sangat memadai untuk dataset yang kami kelola."
 * =====================================================================================
 */
typedef struct Paper {
    char id[100];
    char title[1000];
    char incitation[100];
    char author[1000];
    int year;
} Paper;

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data Node (Single Linked List)
 * =====================================================================================
 * "Struktur data pertama yang kami gunakan adalah Single Linked List (SLL).
 * SLL kami pilih sebagai 'kontainer' awal untuk menampung data yang dibaca dari file.
 * Alasan utamanya adalah karena proses memuat data dari file bersifat sekuensial,
 * dan SLL sangat efisien dalam operasi penambahan data di akhir (append),
 * terutama karena kami mengelola pointer 'tail' yang memberikan kompleksitas O(1).
 * Dari SLL inilah struktur data lain yang lebih kompleks seperti Tree dan AVL dibentuk."
 * =====================================================================================
 */
typedef struct Node {
    Paper data;
    struct Node* next;
} Node;

// --- Prototipe Fungsi ---
Node* loadDataFromFile(const char* filename);
void printAllPapers(Node* head);
Node* searchPaperById(Node* head, const char* id);
void freeList(Node* head);
void print_truncated(const char* text, int max_len);

#endif