// include/avl.h
#ifndef AVL_H
#define AVL_H

#include "sll.h"

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data AVLNode (Pohon Pencarian)
 * =====================================================================================
 * "Untuk mengoptimalkan fitur pencarian, kami mengimplementasikan AVL Tree.
 * Ini adalah Pohon Pencarian Biner (BST) yang bisa menyeimbangkan dirinya sendiri.
 * - 'left' & 'right': Pointer standar BST.
 * - 'height': Atribut tambahan yang krusial. Tinggi node ini digunakan untuk
 * menghitung 'Balance Factor' dan memastikan pohon tidak pernah miring,
 * sehingga performa pencarian tetap terjaga di O(log n)."
 * =====================================================================================
 */
typedef struct AVLNode {
    Paper data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// --- Prototipe Fungsi ---
AVLNode* insertAVL(AVLNode* node, Paper data, int visualize);
AVLNode* searchAVL(AVLNode* root, const char* id);
AVLNode* buildAVLTreeFromSLL(Node* head, int visualize);
void freeAVLTree(AVLNode* root);
void displayAVLAnalysis(AVLNode* root);

// Fungsi performSearchRace telah dihapus.

#endif