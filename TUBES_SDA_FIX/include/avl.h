// include/avl.h

#ifndef AVL_H
#define AVL_H

#include "sll.h"

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

// FUNGSI BARU: Fungsi utama untuk menampilkan analisis lengkap (diagram + deskripsi)
void displayAVLAnalysis(AVLNode* root);

#endif