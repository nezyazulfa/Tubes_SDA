// include/avl.h

#ifndef AVL_H
#define AVL_H

#include "sll.h" // Kita butuh definisi struct Paper

/*
 * =====================================================================================
 * Penjelasan Struktur Data AVLNode
 * =====================================================================================
 * Struct 'AVLNode' adalah komponen pembangun dari AVL Tree.
 * - 'data': Menyimpan satu buah data 'Paper'.
 * - 'left', 'right': Pointer ke anak kiri dan kanan, membentuk struktur BST.
 * - 'height': Menyimpan tinggi dari node ini. Informasi tinggi sangat krusial
 * untuk menghitung balance factor dan menjaga keseimbangan pohon.
 * =====================================================================================
 */
typedef struct AVLNode {
    Paper data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// --- Prototipe Fungsi ---

// Fungsi utama untuk menyisipkan node baru ke dalam AVL tree.
AVLNode* insertAVL(AVLNode* node, Paper data);

// Fungsi untuk mencari paper berdasarkan ID di dalam AVL tree.
AVLNode* searchAVL(AVLNode* root, const char* id);

// Fungsi untuk membangun AVL tree dari sebuah Single Linked List.
AVLNode* buildAVLTreeFromSLL(Node* head);

// Fungsi untuk membebaskan semua memori yang dialokasikan untuk AVL tree.
void freeAVLTree(AVLNode* root);

#endif