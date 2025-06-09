// include/sll.h

#ifndef SLL_H
#define SLL_H

/*
 * =====================================================================================
 * Penjelasan Struktur Data Paper
 * =====================================================================================
 * Struct 'Paper' ini adalah unit data dasar dalam program kita.
 * Ia menyimpan semua informasi relevan tentang sebuah karya ilmiah,
 * termasuk ID uniknya, judul, ID paper yang disitasinya ('incitation'),
 * penulis, dan tahun terbit.
 *
 * Mengapa menggunakan char array dengan ukuran tetap (misal, id[100])?
 * Ini adalah pendekatan sederhana untuk alokasi memori. Kita mengasumsikan
 * panjang data tidak akan melebihi ukuran ini. Untuk program skala besar,
 * alokasi memori dinamis (malloc untuk setiap string) bisa lebih efisien
 * dalam penggunaan memori, namun lebih kompleks untuk dikelola.
 * =====================================================================================
 */
typedef struct Paper {
    char id[100];         // ID unik dari paper (string heksadesimal)
    char title[1000];     // Judul paper
    char incitation[100]; // ID dari paper yang DI-SITASI oleh paper ini
    char author[1000];    // Nama-nama penulis
    int year;             // Tahun terbit
} Paper;


/*
 * =====================================================================================
 * Penjelasan Struktur Data Node (Single Linked List)
 * =====================================================================================
 * Struct 'Node' adalah komponen pembangun dari Single Linked List (SLL).
 * - 'data': Menyimpan satu buah data 'Paper'.
 * - 'next': Sebuah pointer yang menunjuk ke 'Node' berikutnya dalam urutan list.
 * Jika ini adalah node terakhir, 'next' akan bernilai NULL.
 *
 * Mengapa SLL? SLL dipilih sebagai struktur data utama untuk menyimpan
 * data awal dari file karena proses penambahannya (insert di akhir) sangat
 * efisien (O(1) jika kita menyimpan pointer ke 'tail') dan proses memuat
 * data dari file bersifat sekuensial.
 * =====================================================================================
 */
typedef struct Node {
    Paper data;
    struct Node* next;
} Node;


// --- Prototipe Fungsi ---

// Fungsi untuk memuat data dari file ke dalam SLL.
Node* loadDataFromFile(const char* filename);

// Fungsi untuk menampilkan semua paper dalam SLL.
void printAllPapers(Node* head);

// Fungsi untuk mencari paper berdasarkan ID string-nya di SLL (digunakan untuk fallback).
Node* searchPaperById(Node* head, const char* id);

// Fungsi untuk membebaskan semua memori yang dialokasikan untuk SLL.
void freeList(Node* head);

// Fungsi helper untuk mencetak string dengan pemotongan jika terlalu panjang.
void print_truncated(const char* text, int max_len);

#endif