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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Menu untuk analisis dan visualisasi AVL Tree (tanpa adu cepat)
void analysisMenu(AVLNode* avlRoot, Node* paperList) {
    int choice;
    clearScreen();
    printf("+=================================================================+\n");
    printf("|          VISUALISASI & ANALISIS STRUKTUR INDEKS (AVL)           |\n");
    printf("+=================================================================+\n\n");
    printf("  Fitur ini membantu memahami bagaimana struktur data indeks pencarian\n");
    printf("  dibangun dan dijaga agar tetap seimbang untuk pencarian cepat.\n\n");
    printf("  [1] Tampilkan Struktur Indeks Final (Diagram & Deskripsi)\n");
    printf("  [2] Tampilkan Proses Pembangunan Indeks Step-by-Step\n");
    printf("  [0] Kembali ke Menu Utama\n\n");
    printf("  >> Pilihan Anda: ");
    
    if (scanf("%d", &choice) != 1) { choice = -1; }
    clearInputBuffer();

    switch(choice) {
        case 1:
            clearScreen();
            printf("+=================================================================+\n");
            printf("|               ANALISIS STRUKTUR INDEKS FINAL (AVL)              |\n");
            printf("+=================================================================+\n\n");
            displayAVLAnalysis(avlRoot);
            break;
        case 2:
            clearScreen();
            printf("+=================================================================+\n");
            printf("|         DEMONSTRASI PEMBANGUNAN INDEKS STEP-BY-STEP             |\n");
            printf("+=================================================================+\n\n");
            AVLNode* tempVisualAVL = buildAVLTreeFromSLL(paperList, 1);
            freeAVLTree(tempVisualAVL);
            break;
        case 0: return;
        default: printf("\n  [ERROR] Pilihan tidak valid.\n"); break;
    }
    printf("\n\n  Tekan Enter untuk kembali...");
    getchar();
}

int main() {
    clearScreen();
    printf("================================================================================\n");
    printf("||              SISTEM REFERENSI & MANAJEMEN PAPER ILMIAH                     ||\n");
    printf("================================================================================\n\n");

    printf("Mempersiapkan sistem...\n");
    printf("  - Memuat data dari file... ");
    Node* paperList = loadDataFromFile("TUBES_SDA_FIX/dataset/data.txt");
    if (!paperList) {
        printf("GAGAL.\n");
        printf("  Pastikan file 'data.txt' ada di direktori yang sama.\n");
        getchar();
        return 1;
    }
    printf("OK.\n");

    printf("  - Membangun indeks pencarian (AVL Tree)... ");
    AVLNode* avlRoot = buildAVLTreeFromSLL(paperList, 0);
    printf("OK.\n");

    printf("\nSistem siap digunakan.\n");
    printf("Tekan Enter untuk masuk ke menu utama...");
    getchar();

    StackNode* bookmarkStack = NULL;
    Queue citationQueue;
    initQueue(&citationQueue);

    int choice;
    char id[100];

    while (1) {
        clearScreen();
        printf("+==============================================================================+\n");
        printf("|                        MENU UTAMA - SISTEM MANAJEMEN PAPER                   |\n");
        printf("+==============================================================================+\n\n");

        printf("--- Fitur Utama ---\n");
        printf("  [1] Cari Paper Berdasarkan ID   | Cepat, via Indeks AVL Tree (O(log n))\n");
        printf("  [2] Dapatkan Rekomendasi        | Berdasarkan Sitasi (Queue)\n");
        printf("  [3] Visualisasi Jejaring Sitasi | Lihat Hubungan Antar Paper (General Tree)\n\n");

        printf("--- Analisis & Manajemen ---\n");
        printf("  [4] Visualisasi Indeks Pencarian| Pahami cara kerja pencarian cepat (AVL)\n");
        printf("  [5] Manajemen Bookmark          | Kelola paper pilihan (Stack)\n\n");

        printf("--- Tampilan Data Lainnya ---\n");
        printf("  [8] Tampilkan Semua (Urutan File)| Metode: SLL\n");
        printf("  [9] Tampilkan Semua (Urut Tahun)| Metode: DLL + Bubble Sort\n\n");
        
        printf("--- Sistem ---\n");
        printf("  [10] Keluar\n\n");

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
                clearScreen();
                printf("+=================================================================+\n");
                printf("|               CARI PAPER BERDASARKAN ID (VIA AVL)               |\n");
                printf("+=================================================================+\n\n");
                printf("  Masukkan ID paper yang dicari: ");
                scanf("%99s", id);
                clearInputBuffer();
                
                AVLNode* foundNode = searchAVL(avlRoot, id);
                if (foundNode) {
                    printf("\n  --- Paper Ditemukan ---\n");
                    printf("    ID           : %s\n", foundNode->data.id);
                    printf("    Judul        : %s\n", foundNode->data.title);
                    printf("    Mensitasi ID : %s\n", foundNode->data.incitation);
                    printf("    Author       : %s\n", foundNode->data.author);
                    printf("    Tahun        : %d\n", foundNode->data.year);
                    printf("  -----------------------\n");
                } else {
                    printf("\n  [INFO] Paper dengan ID '%s' tidak ditemukan.\n", id);
                }
                break;
            }
            case 2: {
                clearScreen();
                printf("+=================================================================+\n");
                printf("|                 REKOMENDASI PAPER BERDASARKAN SITASI              |\n");
                printf("+=================================================================+\n\n");
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
                break;
            }
            case 3:
                clearScreen();
                printf("+=================================================================+\n");
                printf("|                    VISUALISASI POHON SITASI                     |\n");
                printf("+=================================================================+\n\n");
                if (!paperList) { printf("  [INFO] Daftar paper kosong.\n"); }
                else {
                    printf("  Membangun dan menampilkan struktur pohon, mohon tunggu...\n");
                    TreeNode* citationTreeRoot = buildCitationTree(paperList);
                    printTreeVisual(citationTreeRoot);
                    freeTree(citationTreeRoot);
                }
                break;
            case 4:
                analysisMenu(avlRoot, paperList);
                continue; 
            case 5: {
                int bookmarkChoice;
                clearScreen();
                printf("+=================================================================+\n");
                printf("|                        MANAJEMEN BOOKMARK                       |\n");
                printf("+=================================================================+\n\n");
                printf("  [1] Tambah Bookmark\n");
                printf("  [2] Tampilkan Bookmark\n");
                printf("  [3] Simpan Bookmark ke File\n");
                printf("  [0] Kembali ke Menu Utama\n\n");
                printf("  >> Pilihan Anda: ");
                scanf("%d", &bookmarkChoice);
                clearInputBuffer();
                switch(bookmarkChoice) {
                    case 1:
                        printf("\n  Masukkan ID paper yang ingin dibookmark: ");
                        scanf("%99s", id);
                        clearInputBuffer();
                        Node* found = searchPaperById(paperList, id);
                        if (found) {
                            push(&bookmarkStack, found->data);
                            printf("\n  [OK] Paper '[%s]' berhasil dibookmark!\n", found->data.id);
                        } else {
                            printf("\n  [INFO] Paper dengan ID '%s' tidak ditemukan.\n", id);
                        }
                        break;
                    case 2:
                        printBookmarks(bookmarkStack);
                        break;
                    case 3:
                        saveBookmarksToFile(bookmarkStack, "bookmark.txt");
                        break;
                    default:
                        break;
                }
                break;
            }
            case 8:
                clearScreen();
                printf("+=================================================================+\n");
                printf("|             TAMPILKAN SEMUA (URUTAN ORIGINAL)                   |\n");
                printf("+=================================================================+\n");
                printAllPapers(paperList);
                break;
            case 9: {
                clearScreen();
                printf("+=================================================================+\n");
                printf("|               TAMPILKAN SEMUA (URUT TAHUN)                      |\n");
                printf("+=================================================================+\n\n");
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
                printf("\n  [ERROR] Pilihan tidak valid.\n");
        }

        if (choice != 10) {
            printf("\n\n  Tekan Enter untuk kembali ke Menu Utama...");
            getchar();
        }
    }
    return 0;
}