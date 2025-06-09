// src/avl.c

/*
 * File ini berisi implementasi lengkap dari struktur data AVL Tree.
 * Tujuannya adalah untuk menyediakan operasi pencarian (search) yang sangat
 * cepat dengan kompleksitas waktu O(log n).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"

// --- Fungsi Helper Internal ---

// Fungsi untuk mendapatkan tinggi sebuah node.
int height(AVLNode* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Fungsi untuk mendapatkan nilai maksimum dari dua integer.
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fungsi untuk membuat node AVL baru.
AVLNode* createAVLNode(Paper data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // Node baru awalnya memiliki tinggi 1
    return node;
}


/*
 * =====================================================================================
 * Penjelasan Fungsi Rotasi
 * =====================================================================================
 * Rotasi adalah mekanisme inti untuk menyeimbangkan kembali pohon.
 * Ada dua jenis rotasi dasar: Kanan (right) dan Kiri (left).
 *
 * Right Rotate (Rotasi Kanan):
 * Dilakukan ketika pohon menjadi "berat sebelah kiri".
 * Node 'y' yang tidak seimbang akan turun, dan anak kirinya 'x' akan naik
 * menjadi root baru dari subtree ini.
 *
 * y                               x
 * / \                             /   \
 * x   T3                          T1    y
 * / \       --Rotasi Kanan-->       / \
 * T1  T2                            T2  T3
 *
 * Left Rotate (Rotasi Kiri):
 * Kebalikan dari rotasi kanan, dilakukan saat pohon "berat sebelah kanan".
 *
 * x                               y
 * / \                             /   \
 * T1  y                           x     T3
 * / \     --Rotasi Kiri-->    / \
 * T2  T3                      T1  T2
 * =====================================================================================
 */
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Lakukan rotasi
    x->right = y;
    y->left = T2;

    // Perbarui tinggi node yang berubah
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Kembalikan root baru dari subtree
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Lakukan rotasi
    y->left = x;
    x->right = T2;

    // Perbarui tinggi
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Kembalikan root baru
    return y;
}


// Fungsi untuk mendapatkan Balance Factor dari sebuah node.
int getBalance(AVLNode* N) {
    if (N == NULL)
        return 0;
    return height(N->right) - height(N->left);
}


/*
 * =====================================================================================
 * Penjelasan Fungsi insertAVL (SANGAT PENTING)
 * =====================================================================================
 * Fungsi ini menyisipkan sebuah paper ke dalam AVL Tree secara rekursif.
 *
 * Algoritma:
 * 1. LAKUKAN PENYISIPAN BST STANDAR:
 * Secara rekursif, cari posisi yang tepat untuk node baru berdasarkan perbandingan
 * ID string (`strcmp`), lalu sisipkan di sana.
 *
 * 2. PERBARUI TINGGI:
 * Setelah menyisipkan, perbarui tinggi dari node leluhur (ancestor).
 *
 * 3. HITUNG BALANCE FACTOR:
 * Hitung balance factor dari node leluhur ini untuk memeriksa apakah pohon
 * menjadi tidak seimbang setelah penyisipan.
 *
 * 4. LAKUKAN ROTASI JIKA PERLU (PROSES REBALANCING):
 * Jika balance factor adalah -2 atau 2, pohon tidak seimbang. Ada 4 kasus:
 * a. Left-Left Case (Berat Kiri-Kiri): Cukup 1x Rotasi Kanan.
 * b. Right-Right Case (Berat Kanan-Kanan): Cukup 1x Rotasi Kiri.
 * c. Left-Right Case (Berat Kiri-Kanan): Rotasi Kiri pada anak kiri, lalu Rotasi Kanan pada node ini.
 * d. Right-Left Case (Berat Kanan-Kiri): Rotasi Kanan pada anak kanan, lalu Rotasi Kiri pada node ini.
 *
 * Proses ini menjamin pohon tetap seimbang setelah setiap penyisipan.
 * =====================================================================================
 */
AVLNode* insertAVL(AVLNode* node, Paper data) {
    // 1. Lakukan penyisipan BST standar
    if (node == NULL)
        return createAVLNode(data);

    if (strcmp(data.id, node->data.id) < 0)
        node->left = insertAVL(node->left, data);
    else if (strcmp(data.id, node->data.id) > 0)
        node->right = insertAVL(node->right, data);
    else // ID yang sama tidak diizinkan di BST
        return node;

    // 2. Perbarui tinggi dari node leluhur ini
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Hitung balance factor
    int balance = getBalance(node);

    // 4. Jika tidak seimbang, lakukan rotasi sesuai 4 kasus

    // Left-Left Case
    if (balance < -1 && strcmp(data.id, node->left->data.id) < 0)
        return rightRotate(node);

    // Right-Right Case
    if (balance > 1 && strcmp(data.id, node->right->data.id) > 0)
        return leftRotate(node);

    // Left-Right Case
    if (balance < -1 && strcmp(data.id, node->left->data.id) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-Left Case
    if (balance > 1 && strcmp(data.id, node->right->data.id) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Kembalikan pointer node (jika tidak ada rotasi)
    return node;
}


// Fungsi untuk mencari node berdasarkan ID (pencarian BST standar)
AVLNode* searchAVL(AVLNode* root, const char* id) {
    // Basis kasus: root adalah NULL atau ID ditemukan di root
    if (root == NULL || strcmp(root->data.id, id) == 0)
        return root;

    // Jika ID yang dicari lebih besar dari ID root, cari di subtree kanan
    if (strcmp(root->data.id, id) < 0)
        return searchAVL(root->right, id);

    // Jika ID yang dicari lebih kecil, cari di subtree kiri
    return searchAVL(root->left, id);
}

// Fungsi pembungkus untuk membangun AVL Tree dari SLL
AVLNode* buildAVLTreeFromSLL(Node* head) {
    AVLNode* root = NULL;
    Node* current = head;
    while (current != NULL) {
        root = insertAVL(root, current->data);
        current = current->next;
    }
    return root;
}

// Membebaskan memori AVL tree secara rekursif (post-order traversal)
void freeAVLTree(AVLNode* root) {
    if (root != NULL) {
        freeAVLTree(root->left);
        freeAVLTree(root->right);
        free(root);
    }
}