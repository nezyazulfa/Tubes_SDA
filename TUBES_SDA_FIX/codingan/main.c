// src/main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sll.h"
#include "../include/dll.h"
#include "../include/tree.h"
#include "../include/stack.h"
#include "../include/queue.h"
#include "../include/avl.h"

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

// Sub-menu untuk analisis dan visualisasi AVL Tree.
void avlMenu(AVLNode* avlRoot, Node* paperList) {
    int choice;
    clearScreen();
    printf("+=================================================================+\n");
    printf("|          ANALISIS, DIAGRAM, & DESKRIPSI AVL TREE                |\n");
    printf("+=================================================================+\n\n");
    printf("  [1] Tampilkan Analisis Pohon AVL Final\n");
    printf("  [2] Tampilkan Proses Pembangunan AVL Step-by-Step\n");
    printf("  [0] Kembali ke Menu Utama\n\n");
    printf("  >> Pilihan Anda: ");

    if (scanf("%d", &choice) != 1) {
        choice = -1; // Menandai pilihan tidak valid
    }
    clearInputBuffer();

    switch(choice) {
        case 1:
            printf("\n\n--- ANALISIS POHON AVL FINAL ---\n");
            displayAVLAnalysis(avlRoot);
            break;
        case 2:
            printf("\n\n--- MEMULAI ULANG PROSES PEMBANGUNAN UNTUK DEMONSTRASI ---\n");
            // Pohon ini dibangun ulang hanya untuk tujuan demonstrasi step-by-step
            // dan langsung dibebaskan memorinya setelah selesai.
            AVLNode* tempVisualAVL = buildAVLTreeFromSLL(paperList, 1);
            freeAVLTree(tempVisualAVL);
            break;
        case 0:
            return; // Kembali ke menu utama
        default:
            printf("\n  [ERROR] Pilihan tidak valid.\n");
            break;
    }
    printf("\n\n  Tekan Enter untuk kembali...");
    getchar();
}


int main() {
    // Pastikan path ke data.txt sudah benar
    Node* paperList = loadDataFromFile("data.txt");
    if (!paperList) {
        printf("GAGAL MEMUAT DATA. Pastikan file 'data.txt' ada di direktori yang sama dengan program.\n");
        return 1;
    }

    // Saat program dimulai, AVL tree langsung dibangun secara diam-diam (tanpa visualisasi).
    // Ini berfungsi sebagai "indeks" untuk membuat semua operasi pencarian menjadi sangat cepat.
    printf("Membangun indeks pencarian (AVL Tree)...");
    AVLNode* avlRoot = buildAVLTreeFromSLL(paperList, 0); // 0 = mode senyap
    printf(" Selesai.\n");
    printf("Tekan Enter untuk memulai...");
    getchar();


    // Inisialisasi struktur data lain yang dibutuhkan
    StackNode* bookmarkStack = NULL;
    Queue citationQueue;
    initQueue(&citationQueue);

    int choice;
    char id[100];

    // Loop utama program
    while (1) {
        clearScreen();
        printf("+=================================================================+\n");
        printf("|              SISTEM REFERENSI & MANAJEMEN PAPER                 |\n");
        printf("+=================================================================+\n\n");

        printf("  +---------------------------------+\n");
        printf("  |  Pencarian & Rekomendasi        |\n");
        printf("  +---------------------------------+\n");
        printf("  | [1] Cari Paper Berdasarkan ID   |    ---- AVL Tree (O(log n))\n");
        printf("  | [2] Rekomendasi Paper (Sitasi)  |    ---- Queue\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Eksplorasi & Analisis          |\n");
        printf("  +---------------------------------+\n");
        printf("  | [3] Visualisasi Pohon Sitasi    |    ---- General Tree\n");
        printf("  | [4] Analisis & Diagram AVL      |    ---- AVL Tree\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Manajemen Bookmark             |\n");
        printf("  +---------------------------------+\n");
        printf("  | [5] Tambah Paper ke Bookmark    |    ---- Stack\n");
        printf("  | [6] Tampilkan Daftar Bookmark   |    ---- Stack\n");
        printf("  | [7] Simpan Bookmark ke File     |    ---- Stack\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Tampilan Daftar Keseluruhan    |\n");
        printf("  +---------------------------------+\n");
        printf("  | [8] Tampilkan Semua (Urutan Asli)|    ---- Single Linked List\n");
        printf("  | [9] Tampilkan Semua (Urut Tahun)|    ---- Double Linked List\n");
        printf("  +---------------------------------+\n\n");
        printf("  +---------------------------------+\n");
        printf("  |  Lain-lain                      |\n");
        printf("  +---------------------------------+\n");
        printf("  | [10] Keluar dari Program        |\n");
        printf("  +---------------------------------+\n\n");

        printf("  >> Masukkan Pilihan Anda [1-10]: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n  [ERROR] Input tidak valid. Harap masukkan angka.\n");
            clearInputBuffer();
            printf("  Tekan Enter untuk melanjutkan...");
            getchar();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                printf("\n  **** CARI PAPER BERDASARKAN ID (VIA AVL TREE) ****\n\n");
                printf("  Masukkan ID paper yang dicari: ");
                scanf("%99s", id);
                clearInputBuffer();
                
                AVLNode* foundNode = searchAVL(avlRoot, id);
                if (foundNode) {
                    printf("\n  --- Detail Lengkap Paper ---\n");
                    printf("    ID           : %s\n", foundNode->data.id);
                    printf("    Judul        : %s\n", foundNode->data.title);
                    printf("    Mensitasi ID : %s\n", foundNode->data.incitation);
                    printf("    Author       : %s\n", foundNode->data.author);
                    printf("    Tahun        : %d\n", foundNode->data.year);
                    printf("  -----------------------------\n");
                } else {
                    printf("\n  [INFO] Paper dengan ID '%s' tidak ditemukan.\n", id);
                }
                break;
            }
            case 2:
            {
                printf("\n  **** REKOMENDASI PAPER (SITASI) ****\n\n");
                freeQueue(&citationQueue);
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
                    if (strcmp(currentPaperInList->data.incitation, targetId) == 0) {
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
                     printf("\n  [N]ext | [L]ainnya Kembali ke Menu : ");
                     scanf(" %c", &nextChoiceChar);
                     clearInputBuffer();
                 } while (nextChoiceChar == 'n' || nextChoiceChar == 'N');
            }
            break;

            case 3:
                printf("\n  **** VISUALISASI POHON SITASI ****\n\n");
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
                avlMenu(avlRoot, paperList);
                break;

            case 5:
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

            case 6:
                printBookmarks(bookmarkStack);
                break;

            case 7:
                printf("\n  **** SIMPAN BOOKMARK KE FILE ****\n\n");
                saveBookmarksToFile(bookmarkStack, "bookmark.txt");
                break;

            case 8:
                printAllPapers(paperList);
                break;
            
            case 9:
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
                    sortDLLByYear(&dllList, 1);
                    printDLL(dllList);
                } else {
                    sortDLLByYear(&dllList, 0);
                    DNode *tail = dllList;
                    if(tail) { while(tail->next) tail = tail->next; }
                    printDLLBackward(tail);
                }
                freeDLL(dllList);
            }
            break;
            
            case 10:
                printf("\n  Membersihkan memori sebelum keluar...\n");
                freeList(paperList);
                freeStack(bookmarkStack);
                freeQueue(&citationQueue);
                freeAVLTree(avlRoot);
                printf("  Semua memori telah dibebaskan. Sampai jumpa!\n\n");
                return 0;

            default:
                printf("\n  [ERROR] Pilihan tidak valid. Harap pilih nomor menu antara 1-10.\n");
        }

        if (choice != 10) {
            printf("\n\n  Tekan Enter untuk kembali ke Menu Utama...");
            getchar();
        }
    }
    return 0;
}