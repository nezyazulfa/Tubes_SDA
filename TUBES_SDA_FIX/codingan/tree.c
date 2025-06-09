// src/tree.c

/*
 * File ini mengelola semua yang berhubungan dengan struktur data Tree (Pohon Sitasi).
 * Pohon ini merepresentasikan jaringan sitasi, di mana sebuah paper
 * yang mensitasi paper lain akan menjadi 'anak' dari paper yang disitasi.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"

// Fungsi internal untuk membuat node pohon baru.
TreeNode* createTreeNode(Paper data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode) {
        newNode->data = data;
        newNode->child = NULL;
        newNode->sibling = NULL;
    }
    return newNode;
}

// Menambahkan 'child' ke 'parent' menggunakan representasi First-Child, Next-Sibling.
void addChild(TreeNode* parent, TreeNode* child) {
    if (!parent->child) {
        // Jika parent belum punya anak, ini jadi anak pertamanya.
        parent->child = child;
    } else {
        // Jika sudah punya, cari anak terakhir dan tambahkan sebagai saudaranya.
        TreeNode* temp = parent->child;
        while (temp->sibling) {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

/*
 * =====================================================================================
 * Penjelasan Fungsi buildCitationTree (SANGAT PENTING)
 * =====================================================================================
 * Fungsi ini adalah jantung dari fitur visualisasi pohon. Ia membangun pohon sitasi
 * dari daftar paper (SLL) yang tidak terurut.
 *
 * Masalah: Jika kita memproses data secara sekuensial, kita mungkin menemukan
 * sebuah "paper anak" sebelum "paper induk"-nya. Akibatnya, kita tidak tahu di
 * mana harus menempatkan si anak. Ini disebut "masalah node yatim" (orphan node).
 *
 * Solusi: Algoritma Dua Pass (Two-Pass Algorithm).
 *
 * Pass 1: "Membuat Peta Node"
 * - Tujuan: Membuat `TreeNode` untuk SETIAP paper di SLL, tanpa menghubungkannya.
 * - Proses: Kita iterasi seluruh SLL dan membuat `TreeNode` untuk setiap paper.
 * Semua pointer ke `TreeNode` ini disimpan dalam sebuah array sementara (`node_map`).
 * - Hasil: Sebuah "kolam" node yang siap dihubungkan. Setiap node, baik induk
 * maupun anak, DIJAMIN sudah ada di dalam `node_map`.
 *
 * Pass 2: "Menghubungkan Node"
 * - Tujuan: Membangun struktur pohon dengan menghubungkan node-node dari `node_map`.
 * - Proses: Iterasi `node_map`. Untuk setiap `currentNode`:
 * 1. Cek `incitation` ID-nya.
 * 2. Jika sitasinya "-" (root), tambahkan ke level atas pohon (sebagai root atau
 * saudara dari root yang sudah ada).
 * 3. Jika ada ID sitasi, cari `parentNode` di dalam `node_map` dengan ID yang cocok.
 * 4. Karena semua node sudah dibuat di Pass 1, pencarian ini PASTI berhasil
 * (jika datanya konsisten).
 * 5. Hubungkan `currentNode` sebagai anak dari `parentNode` menggunakan `addChild`.
 * =====================================================================================
 */
TreeNode* buildCitationTree(Node* paperList) {
    if (!paperList) return NULL;

    int count = 0;
    Node* counter = paperList;
    while (counter) {
        count++;
        counter = counter->next;
    }
    if (count == 0) return NULL;

    // --- PASS 1: Membuat Peta Node ---
    TreeNode** node_map = (TreeNode**)malloc(count * sizeof(TreeNode*));
    if (!node_map) return NULL;

    Node* current = paperList;
    for (int i = 0; i < count; i++) {
        node_map[i] = createTreeNode(current->data);
        current = current->next;
    }

    // --- PASS 2: Menghubungkan Node ---
    TreeNode* root = NULL;

    for (int i = 0; i < count; i++) {
        TreeNode* currentNode = node_map[i];
        const char* citationId = currentNode->data.incitation;

        if (strcmp(citationId, "-") == 0 || strcmp(citationId, "NULL") == 0) {
            if (root == NULL) {
                root = currentNode;
            } else {
                TreeNode* temp = root;
                while (temp->sibling) {
                    temp = temp->sibling;
                }
                temp->sibling = currentNode;
            }
        } else {
            TreeNode* parentNode = NULL;
            for (int j = 0; j < count; j++) {
                if (strcmp(node_map[j]->data.id, citationId) == 0) {
                    parentNode = node_map[j];
                    break;
                }
            }
            if (parentNode) {
                addChild(parentNode, currentNode);
            } else {
                // printf("Peringatan: Paper '%s' merujuk sitasi '%s' yang tidak ada di dataset.\n", currentNode->data.id, citationId);
            }
        }
    }

    free(node_map);
    return root;
}

/*
 * =====================================================================================
 * Penjelasan Fungsi Visualisasi Pohon (printTreeVisual & printTreeRecursive)
 * =====================================================================================
 * Tampilan pohon yang rapi dibuat secara rekursif.
 *
 * `printTreeVisual`: Fungsi pembungkus (wrapper) yang memulai proses. Ia akan
 * menangani kasus jika ada beberapa pohon (hutan) dengan mencetak setiap root
 * dan memulai proses rekursif untuk anak-anaknya.
 *
 * `printTreeRecursive`: Fungsi rekursif inti.
 * - `prefix`: Ini adalah string yang berisi garis-garis konektor vertikal (`|   `
 * atau `    `). String ini dibangun dan diwariskan ke level rekursi yang
 * lebih dalam untuk menjaga alignment.
 * - `isLast`: Flag boolean ini sangat penting. Ia memberitahu fungsi apakah node
 * yang sedang dicetak adalah saudara terakhir di levelnya.
 * - Jika `true`, konektor yang dicetak adalah "`-- " (menandakan akhir cabang).
 * - Jika `false`, konektor yang dicetak adalah "|-- ".
 * Flag ini juga menentukan prefix untuk level selanjutnya: spasi ("    ") jika
 * terakhir (karena tidak ada lagi saudara di bawahnya), atau garis ("|   ") jika
 * masih ada saudara yang akan dicetak.
 * =====================================================================================
 */
void printTreeRecursive(TreeNode* node, char* prefix, int isLast) {
    if (!node) return;

    printf("%s", prefix);
    printf(isLast ? "`-- " : "|-- ");

    // Mencetak info node dengan format yang lebih rapi
    printf("[%s] ", node->data.id);
    print_truncated(node->data.title, 60);
    printf(" (%d)\n", node->data.year);

    char newPrefix[1000];
    sprintf(newPrefix, "%s%s", prefix, isLast ? "    " : "|   ");

    TreeNode* child = node->child;
    while (child) {
        printTreeRecursive(child, newPrefix, child->sibling == NULL);
        child = child->sibling;
    }
}

void printTreeVisual(TreeNode* root) {
    if (!root) {
        printf("\n  [INFO] Pohon kosong atau gagal dibangun.\n");
        return;
    }

    while (root) {
        printf("\n* [%s] ", root->data.id);
        print_truncated(root->data.title, 70);
        printf(" (%d)\n", root->data.year);

        TreeNode* child = root->child;
        while (child) {
            printTreeRecursive(child, "", child->sibling == NULL);
            child = child->sibling;
        }

        root = root->sibling;
        if(root) {
            printf("\n------------------------------------------------------\n");
            printf("--- ROOT POHON BERIKUTNYA ---\n");
        }
    }
}

// Membebaskan memori pohon secara rekursif.
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->child);
    freeTree(root->sibling);
    free(root);
}