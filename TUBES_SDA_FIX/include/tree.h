// include/tree.h

#ifndef TREE_H
#define TREE_H

#include "sll.h"

/*
 * =====================================================================================
 * Penjelasan Struktur Data TreeNode
 * =====================================================================================
 * Struct 'TreeNode' digunakan untuk membangun pohon sitasi.
 * Ini bukan pohon biner biasa, melainkan pohon general (n-ary tree), di mana
 * satu node bisa memiliki banyak anak.
 *
 * Representasi yang digunakan adalah "First-Child, Next-Sibling".
 * - 'data': Menyimpan satu buah data 'Paper'.
 * - 'child': Pointer yang menunjuk ke ANAK PERTAMA dari node ini.
 * - 'sibling': Pointer yang menunjuk ke SAUDARA (node lain yang memiliki
 * induk yang sama) berikutnya dari node ini.
 *
 * Keuntungan: Representasi ini memungkinkan kita membangun pohon dengan jumlah
 * anak yang dinamis menggunakan struktur node yang tetap (hanya 2 pointer),
 * mirip seperti linked list, yang sangat efisien dalam penggunaan memori.
 * =====================================================================================
 */
typedef struct TreeNode {
    Paper data;
    struct TreeNode* child;
    struct TreeNode* sibling;
} TreeNode;

// Fungsi untuk membangun pohon dari SLL.
TreeNode* buildCitationTree(Node* paperList);

// Fungsi untuk menampilkan visualisasi pohon.
void printTreeVisual(TreeNode* root);

// Fungsi untuk membebaskan semua memori yang dialokasikan untuk pohon.
void freeTree(TreeNode* root);

#endif