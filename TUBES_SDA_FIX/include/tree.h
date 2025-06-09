// include/tree.h
#ifndef TREE_H
#define TREE_H

#include "sll.h"

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data TreeNode (Pohon Sitasi)
 * =====================================================================================
 * "Untuk memvisualisasikan jejaring sitasi, kami menggunakan struktur data pohon
 * general (n-ary tree), karena satu paper bisa disitasi oleh banyak paper lain.
 * Kami mengimplementasikannya dengan representasi 'First-Child, Next-Sibling'.
 * - 'child': Menunjuk ke anak PERTAMA.
 * - 'sibling': Menunjuk ke saudara berikutnya (paper lain yang mensitasi induk yang sama).
 * Keuntungan metode ini adalah efisiensi memori; setiap node hanya butuh dua pointer
 * terlepas dari berapa banyak anaknya, mirip seperti linked list."
 * =====================================================================================
 */
typedef struct TreeNode {
    Paper data;
    struct TreeNode* child;
    struct TreeNode* sibling;
} TreeNode;

TreeNode* buildCitationTree(Node* paperList);
void printTreeVisual(TreeNode* root);
void freeTree(TreeNode* root);

#endif