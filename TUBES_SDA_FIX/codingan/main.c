// src/main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sll.h"
#include "../include/dll.h"
#include "../include/tree.h"
#include "../include/stack.h"
#include "../include/queue.h"

// Fungsi untuk membersihkan buffer input setelah scanf.
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi untuk membersihkan layar konsol (cross-platform).
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    // GANTI PATH INI SESUAI LOKASI data.txt DI KOMPUTER ANDA
    Node* paperList = loadDataFromFile("data.txt");
    if (!paperList) {
        printf("GAGAL MEMUAT DATA. Pastikan file 'data.txt' ada di direktori yang sama.\n");
        return 1;
    }

    // Inisialisasi struktur data lain
    StackNode* bookmarkStack = NULL;
    Queue citationQueue;
    initQueue(&citationQueue);

    int choice;
    char id[100]; // ID kembali menjadi string

    // Main loop program
    while (1) {
        clearScreen();
        printf("+=================================================================+\n");
        printf("|              SISTEM REFERENSI & MANAJEMEN PAPER                 |\n");
        printf("+=================================================================+\n\n");
        
        printf("  +---------------------------------+\n");
        printf("  |  Pencarian & Rekomendasi        |\n");
        printf("  +---------------------------------+\n");
        printf("  | [1] Cari Paper Berdasarkan ID   |    ---- Single Linked List\n");
        printf("  | [2] Rekomendasi Paper (Sitasi)  |    ---- Queue\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Eksplorasi Lanjutan            |\n");
        printf("  +---------------------------------+\n");
        printf("  | [3] Visualisasi Tree            |    ---- Tree\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Manajemen Bookmark             |\n");
        printf("  +---------------------------------+\n");
        printf("  | [4] Tambah Paper ke Bookmark    |    ---- Stack\n");
        printf("  | [5] Tampilkan Daftar Bookmark   |    ---- Stack\n");
        printf("  | [6] Simpan Bookmark ke File     |    ---- Stack\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Tampilan Daftar Keseluruhan    |\n");
        printf("  +---------------------------------+\n");
        printf("  | [7] Tampilkan Semua (Urutan Asli)|    ---- Single Linked List\n");
        printf("  | [8] Tampilkan Semua (Urut Tahun)|    ---- Double Linked List\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Lain-lain                      |\n");
        printf("  +---------------------------------+\n");
        printf("  | [9] Keluar dari Program         |\n");
        printf("  +---------------------------------+\n\n");
                
        printf("  >> Masukkan Pilihan Anda [1-9]: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n  [ERROR] Input tidak valid. Harap masukkan angka.\n");
            clearInputBuffer();
            printf("  Tekan Enter untuk melanjutkan...");
            getchar();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf("\n  **** CARI PAPER BERDASARKAN ID ****\n\n");
                printf("  Masukkan ID paper yang dicari: ");
                scanf("%99s", id); // Baca ID sebagai string
                clearInputBuffer();
                {
                    Node* found = searchPaperById(paperList, id);
                    if (found) {
                        printf("\n  --- Detail Lengkap Paper ---\n");
                        printf("    ID           : %s\n", found->data.id);
                        printf("    Judul        : %s\n", found->data.title);
                        printf("    Mensitasi ID : %s\n", found->data.incitation);
                        printf("    Author       : %s\n", found->data.author);
                        printf("    Tahun        : %d\n", found->data.year);
                        printf("  -----------------------------\n");
                    } else {
                        printf("\n  [INFO] Paper dengan ID '%s' tidak ditemukan.\n", id);
                    }
                }
                break;

            case 2:
            {
                printf("\n  **** REKOMENDASI PAPER (SITASI) ****\n\n");
                freeQueue(&citationQueue); // Kosongkan antrean sebelumnya
                initQueue(&citationQueue);
                
                char targetId[100];
                printf("  Masukkan ID paper basis rekomendasi: ");
                scanf("%99s", targetId);
                clearInputBuffer();

                Node* sourcePaper = searchPaperById(paperList, targetId);
                if (!sourcePaper) {
                    printf("\n  [INFO] Paper dengan ID '%s' tidak ditemukan.\n", targetId);
                    break;
                }
                
                printf("\n  Paper Basis: [%s] ", sourcePaper->data.id);
                print_truncated(sourcePaper->data.title, 60);
                printf("\n  Mencari paper lain yang mensitasi paper ini...\n");

                Node* currentPaperInList = paperList;
                int recommendationsFound = 0;
                while (currentPaperInList) {
                    // Cari paper yang mensitasi targetId
                    if (strcmp(currentPaperInList->data.incitation, targetId) == 0) {
                         // Pastikan tidak merekomendasikan paper itu sendiri
                         if (strcmp(currentPaperInList->data.id, targetId) != 0) {
                            enqueue(&citationQueue, currentPaperInList->data);
                            recommendationsFound++;
                         }
                    }
                    currentPaperInList = currentPaperInList->next;
                }
                
                if (recommendationsFound == 0) {
                     printf("\n  [INFO] Tidak ada paper lain yang ditemukan mensitasi paper ini.\n");
                     break;
                }

                 printf("\n  Ditemukan %d paper. Menampilkan satu per satu:\n", recommendationsFound);
                 char nextChoiceChar;
                 do {
                     if (citationQueue.front == NULL) {
                         printf("\n  -- Tidak ada rekomendasi lagi --\n");
                         break;
                     }
                     Paper recommendedPaper = dequeue(&citationQueue);
                     printf("\n  +-- Rekomendasi Berikutnya --+\n");
                     printf("  | ID      : %s\n", recommendedPaper.id);
                     printf("  | Judul   : ");
                     print_truncated(recommendedPaper.title, 60);
                     printf("\n");
                     printf("  | Tahun   : %d\n", recommendedPaper.year);
                     printf("  +----------------------------+\n");

                     if (citationQueue.front == NULL) {
                         printf("\n  -- Ini adalah rekomendasi terakhir --\n");
                         break;
                     }
                     printf("\n  [N] Next | [Lainnya] Kembali ke Menu : ");
                     scanf(" %c", &nextChoiceChar);
                     clearInputBuffer();
                 } while (nextChoiceChar == 'n' || nextChoiceChar == 'N');
            }
            break;

            case 3:
                printf("\n  **** VISUALISASI JEJARING SITASI (POHON) ****\n\n");
                if (!paperList) {
                    printf("  [INFO] Daftar paper kosong.\n");
                    break;
                }
                printf("  Membangun dan menampilkan struktur pohon, mohon tunggu...\n");
                TreeNode* citationTreeRoot = buildCitationTree(paperList);
                printTreeVisual(citationTreeRoot);
                freeTree(citationTreeRoot);
                break;

            case 4:
                printf("\n  **** TAMBAH PAPER KE BOOKMARK ****\n\n");
                printf("  Masukkan ID paper yang ingin dibookmark: ");
                scanf("%99s", id);
                clearInputBuffer();
                {
                    Node* found = searchPaperById(paperList, id);
                    if (found) {
                        push(&bookmarkStack, found->data);
                        printf("\n  [OK] Paper '[%s]' berhasil dibookmark!\n", found->data.id);
                    } else {
                        printf("\n  [INFO] Paper dengan ID '%s' tidak ditemukan.\n", id);
                    }
                }
                break;

            case 5:
                printBookmarks(bookmarkStack);
                break;

            case 6:
                printf("\n  **** SIMPAN BOOKMARK KE FILE ****\n\n");
                saveBookmarksToFile(bookmarkStack, "bookmark.txt");
                break;

            case 7:
                printAllPapers(paperList);
                break;
            
            case 8:
            {
                printf("\n  **** TAMPILKAN SEMUA PAPER (URUT TAHUN) ****\n\n");
                DNode *dllList = convertSLLtoDLL(paperList);
                if (!dllList) {
                    printf("  [INFO] Gagal mengkonversi ke DLL.\n");
                    break;
                }
                int sort_order_choice;
                printf("  Pilih urutan tahun (1 = Ascending/Terlama, Lainnya = Descending/Terbaru): ");
                if (scanf("%d", &sort_order_choice) != 1) {
                    printf("\n  [ERROR] Input tidak valid.\n");
                    clearInputBuffer();
                    freeDLL(dllList);
                    break;
                }
                clearInputBuffer();

                if (sort_order_choice == 1) {
                    sortDLLByYear(&dllList, 1); // Sort ascending
                    printDLL(dllList);
                } else {
                    sortDLLByYear(&dllList, 0); // Sort descending
                    DNode *tail = dllList;
                    if(tail) { while(tail->next) tail = tail->next; } // Find the new tail
                    printDLLBackward(tail);
                }
                freeDLL(dllList);
            }
            break;
            
            case 9:
                printf("\n  Membersihkan memori sebelum keluar...\n");
                freeList(paperList);
                freeStack(bookmarkStack);
                freeQueue(&citationQueue);
                printf("  Terima kasih telah menggunakan program ini!\n\n");
                return 0;

            default:
                printf("\n  [ERROR] Pilihan tidak valid.\n");
        }

        if (choice != 9) {
            printf("\n\n  Tekan Enter untuk kembali ke Menu Utama...");
            getchar();
        }
    }
    return 0;
}