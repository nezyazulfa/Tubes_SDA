#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Tidak perlu #include <windows.h> jika kita tidak menggunakan SetConsoleOutputCP

#include "../include/sll.h"
#include "../include/dll.h"
#include "../include/tree.h"
#include "../include/stack.h"
#include "../include/queue.h"

// Fungsi untuk membersihkan buffer input
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi untuk membersihkan layar konsol
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    // Tidak ada lagi SetConsoleOutputCP(CP_UTF8);

    Node *paperList = loadDataFromFile("TUBES_SDA_FIX/dataset/data.txt"); 
    if (!paperList) {
        printf("GAGAL MEMUAT DATA: Tidak ada data atau gagal memuat dari 'TUBES_SDA_FIX/dataset/data.txt'.\n");
        printf("Pastikan file ada, formatnya benar, dan path sesuai.\n");
        printf("Program akan keluar.\n");
        return 1;
    }

    StackNode *bookmarkStack = NULL;
    Queue citationQueue;
    initQueue(&citationQueue);

    int choice;
    char id[1000];

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
            printf("\n  [ERROR] Input tidak valid. Harap masukkan angka pilihan menu (1-9).\n");
            clearInputBuffer();
            printf("  Tekan Enter untuk melanjutkan...");
            getchar(); 
            continue;
        }
        clearInputBuffer();

        switch (choice) {
        case 1: // Cari Paper Berdasarkan ID
            printf("\n  **** CARI PAPER BERDASARKAN ID ****\n\n");
            printf("  Masukkan ID paper yang dicari: ");
            scanf("%999s", id); 
            clearInputBuffer();
            {
                Node *found = searchPaperById(paperList, id);
                if (found) {
                    printf("\n  --- Paper Ditemukan ---\n");
                    printf("    ID           : %999s\n", found->data.id);
                    printf("    Judul        : %999s\n", found->data.title);
                    printf("    Mensitasi ID : %999s\n", found->data.incitation);
                    printf("    Author       : %999s\n", found->data.author);
                    printf("    Tahun        : %d\n", found->data.year);
                    printf("  ------------------------\n");
                } else {
                    printf("\n  [INFO] Paper dengan ID '%999s' tidak ditemukan.\n", id);
                }
            }
            break;
        case 2: // Rekomendasi paper
        {
            printf("\n  **** REKOMENDASI PAPER (SITASI) ****\n\n");
            freeQueue(&citationQueue); 
            initQueue(&citationQueue); 

            char targetId[1000];
            printf("  Masukkan ID paper basis rekomendasi: ");
            if (scanf("%999s", targetId) != 1) { 
                printf("\n  [ERROR] Input ID tidak valid.\n");
                clearInputBuffer();
                break;
            }
            clearInputBuffer(); 

            Node *sourcePaper = searchPaperById(paperList, targetId);
            if (!sourcePaper) {
                printf("\n  [INFO] Paper dengan ID '%999s' tidak ditemukan sebagai basis rekomendasi.\n", targetId);
                break;
            }

            printf("\n  Paper Basis: [%999s] %999s\n", sourcePaper->data.id, sourcePaper->data.title);
            printf("  Mencari paper lain yang mensitasi paper ini...\n");

            Node *currentPaperInList = paperList;
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
                printf("\n  [INFO] Tidak ada paper lain yang ditemukan mensitasi '%999s' (%999s).\n", sourcePaper->data.title, sourcePaper->data.id);
                break; 
            }

            printf("\n  Ditemukan %d paper yang mensitasi '%999s'. Menampilkan satu per satu:\n", recommendationsFound, sourcePaper->data.title);

            char nextChoiceChar;
            do {
                if (citationQueue.front == NULL) { 
                    printf("\n  -- Tidak ada rekomendasi lagi untuk ditampilkan --\n");
                    break;
                }

                Paper recommendedPaper = dequeue(&citationQueue);
                printf("\n  +-- Rekomendasi Berikutnya -----------+\n");
                printf("  | ID           : %999s\n", recommendedPaper.id);
                printf("  | Judul        : %999s\n", recommendedPaper.title);
                printf("  | Author       : %999s\n", recommendedPaper.author);
                printf("  | Tahun        : %d\n", recommendedPaper.year);
                printf("  | (Mensitasi ID: %999s)\n", recommendedPaper.incitation);
                printf("  +------------------------------------+\n");


                if (citationQueue.front == NULL) { 
                    printf("\n  -- Ini adalah rekomendasi terakhir --\n");
                    break;
                }

                printf("\n  [N] Next | [Lainnya] Kembali ke Menu : ");
                scanf(" %c", &nextChoiceChar); 
                clearInputBuffer(); 

            } while (nextChoiceChar == 'n' || nextChoiceChar == 'N');
            
            if(citationQueue.front != NULL){
                freeQueue(&citationQueue);
                initQueue(&citationQueue); 
            }
            printf("\n  Selesai menampilkan rekomendasi.\n");
        }
        break;
        case 3: // Visualisasi Tree
        {
            printf("\n  **** VISUALISASI JEJARING SITASI (POHON) ****\n\n");
            if (!paperList) {
                 printf("  [INFO] Daftar paper kosong, tidak bisa membangun tree.\n");
                 break;
            }
            printf("  Membangun struktur pohon sitasi, mohon tunggu...\n");
            TreeNode* citationTreeRoot = buildCitationTree(paperList);
            if (!citationTreeRoot) {
                printf("  [INFO] Gagal membangun pohon sitasi atau tidak ada data root.\n");
                break;
            }
            // printTreeVisual di tree.c akan diubah untuk tampilan ASCII
            printTreeVisual(citationTreeRoot, 0); 
            freeTree(citationTreeRoot); 
        }
        break;
        case 4: // Tambah Bookmark paper
            printf("\n  **** TAMBAH PAPER KE BOOKMARK ****\n\n");
            printf("  Masukkan ID paper yang ingin dibookmark: ");
            scanf("%999s", id);
            clearInputBuffer();
            {
                Node *found = searchPaperById(paperList, id);
                if (found) {
                    push(&bookmarkStack, found->data); 
                    printf("\n  [OK] Paper '%999s' (%999s) berhasil dibookmark!\n", found->data.title, found->data.id);
                } else {
                    printf("\n  [INFO] Paper dengan ID '%999s' tidak ditemukan untuk dibookmark.\n", id);
                }
            }
            break;
        case 5: // Tampilkan bookmark
            // printBookmarks di stack.c akan diubah untuk tampilan ASCII
            printBookmarks(bookmarkStack);
            break;
        case 6: // Simpan bookmark ke file
            printf("\n  **** SIMPAN BOOKMARK KE FILE ****\n\n");
            saveBookmarksToFile(bookmarkStack, "../dataset/bookmark.txt"); 
            break;
        case 7: // Tampilkan semua paper (Urutan Asli)
            // printAllPapers di sll.c akan diubah untuk tampilan ASCII
            printAllPapers(paperList);
            break;
        case 8: // Tampilkan Semua Paper (Terurut Berdasarkan Tahun)
        {
            printf("\n  **** TAMPILKAN SEMUA PAPER (URUT TAHUN) ****\n\n");
            DNode *dllList = convertSLLtoDLL(paperList);
            if (!dllList) {
                printf("  [INFO] Gagal mengkonversi ke DLL (list SLL kosong atau memori penuh).\n");
                break;
            }
            int sort_order_choice;
            printf("  Pilih urutan tahun (1 = Ascending/Terlama dulu, 0 = Descending/Terbaru dulu): ");
            if (scanf("%d", &sort_order_choice) != 1) {
                printf("\n  [ERROR] Input urutan tidak valid.\n");
                clearInputBuffer();
                freeDLL(dllList); 
                break;
            }
            clearInputBuffer();

            sortDLLByYear(&dllList, 1); 

            // printDLL dan printDLLBackward di dll.c akan diubah untuk ASCII
            if (sort_order_choice == 1) { 
                printDLL(dllList);
            } else { 
                DNode *tail = dllList;
                if (tail) { 
                    while (tail->next) { 
                        tail = tail->next;
                    }
                    printDLLBackward(tail); 
                } else {
                    printf("\n  [INFO] List kosong, tidak ada yang bisa ditampilkan setelah diurutkan.\n");
                }
            }
            freeDLL(dllList); 
        }
        break;
        case 9: // Keluar
            printf("\n  Membersihkan memori sebelum keluar...\n");
            freeList(paperList);
            freeStack(bookmarkStack);
            freeQueue(&citationQueue);
            printf("  Terima kasih telah menggunakan program ini. Sampai jumpa!\n\n");
            return 0;
        default:
            printf("\n  [ERROR] Pilihan tidak valid. Silakan pilih nomor menu yang tersedia (1-9).\n");
        }

        if (choice != 9) { 
             printf("\n  Tekan Enter untuk kembali ke Menu Utama...");
             getchar(); 
        }
    }
    return 0; 
}