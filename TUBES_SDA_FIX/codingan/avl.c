// src/avl.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"

// Deklarasi fungsi-fungsi internal
void displayAVLAnalysis(AVLNode* root);

// --- Fungsi Inti AVL (tidak berubah) ---
int height(AVLNode* N) { if (N == NULL) return 0; return N->height; }
int max(int a, int b) { return (a > b) ? a : b; }
int getBalance(AVLNode* N) { if (N == NULL) return 0; return height(N->left) - height(N->right); }

AVLNode* createAVLNode(Paper data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if(!node) return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* insertAVL(AVLNode* node, Paper data, int visualize) {
    if (node == NULL) return createAVLNode(data);

    if (strcmp(data.id, node->data.id) < 0)
        node->left = insertAVL(node->left, data, visualize);
    else if (strcmp(data.id, node->data.id) > 0)
        node->right = insertAVL(node->right, data, visualize);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    int was_unbalanced = (balance > 1 || balance < -1);

    if (visualize && was_unbalanced) {
        printf("\n--> Terdeteksi ketidakseimbangan pada node [%s]. Balance factor: %d\n", node->data.id, balance);
        printf("--> Kondisi Pohon SEBELUM Rotasi:\n");
        displayAVLAnalysis(node);
        printf("    Tekan Enter untuk melanjutkan ke proses rotasi...");
        getchar();
    }
    
    if (balance > 1 && strcmp(data.id, node->left->data.id) < 0) {
        if (visualize) printf("--> KASUS: Left-Left. Melakukan ROTASI KANAN pada node [%s]\n", node->data.id);
        node = rightRotate(node);
    }
    else if (balance < -1 && strcmp(data.id, node->right->data.id) > 0) {
        if (visualize) printf("--> KASUS: Right-Right. Melakukan ROTASI KIRI pada node [%s]\n", node->data.id);
        node = leftRotate(node);
    }
    else if (balance > 1 && strcmp(data.id, node->left->data.id) > 0) {
        if (visualize) printf("--> KASUS: Left-Right. Melakukan ROTASI KIRI-KANAN pada node [%s]\n", node->data.id);
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }
    else if (balance < -1 && strcmp(data.id, node->right->data.id) < 0) {
        if (visualize) printf("--> KASUS: Right-Left. Melakukan ROTASI KANAN-KIRI pada node [%s]\n", node->data.id);
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }

    if (visualize && was_unbalanced) {
        printf("\n--> Kondisi Pohon SETELAH Rotasi:\n");
        displayAVLAnalysis(node);
        printf("    Tekan Enter untuk melanjutkan penyisipan berikutnya...");
        getchar();
    }
    return node;
}

AVLNode* searchAVL(AVLNode* root, const char* id) {
    if (root == NULL || strcmp(root->data.id, id) == 0) return root;
    if (strcmp(id, root->data.id) > 0) return searchAVL(root->right, id);
    return searchAVL(root->left, id);
}

AVLNode* buildAVLTreeFromSLL(Node* head, int visualize) {
    AVLNode* root = NULL;
    Node* current = head;
    int count = 1;
    while (current != NULL) {
        if (visualize) {
            printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< LANGKAH %d >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", count++);
            printf("Menyisipkan Paper: [%s] ", current->data.id);
            print_truncated(current->data.title, 50);
            printf("\n");
        }
        root = insertAVL(root, current->data, visualize);
        current = current->next;
    }
    if (visualize) {
        printf("\n\nPEMBANGUNAN AVL TREE SELESAI.\n");
        printf("Struktur Pohon Final:\n");
        displayAVLAnalysis(root);
    }
    return root;
}

void freeAVLTree(AVLNode* root) {
    if (root != NULL) {
        freeAVLTree(root->left);
        freeAVLTree(root->right);
        free(root);
    }
}

// =====================================================================================
// Implementasi Visualisasi AVL Tree Gabungan (Diagram + Deskripsi)
// =====================================================================================

// --- Bagian 1: Logika untuk Visualisasi Diagram ---
void printAVLRecursive_Graph(AVLNode* node, char* prefix, int isLeft) {
    if (node == NULL) return;
    printf("%s", prefix);
    printf(isLeft ? "`-- " : "|-- ");
    printf("[%s] (BF: %d)\n", node->data.id, getBalance(node));

    char new_prefix[1000];
    sprintf(new_prefix, "%s%s", prefix, isLeft ? "    " : "|   ");

    if (node->left != NULL || node->right != NULL) {
        if (node->right) printAVLRecursive_Graph(node->right, new_prefix, node->left == NULL ? 1 : 0);
        if (node->left) printAVLRecursive_Graph(node->left, new_prefix, 1);
    }
}

// --- Bagian 2: Logika untuk Deskripsi Detail ---
void printAVLRecursive_Desc(AVLNode* node, const char* parent_id) {
    if (node == NULL) return;
    printf("- Node [%s]\n", node->data.id);
    printf("  - Parent        : %s\n", (parent_id == NULL) ? "Ini adalah ROOT" : parent_id);
    printf("  - Anak Kiri     : %s\n", (node->left) ? node->left->data.id : "(Kosong)");
    printf("  - Anak Kanan    : %s\n", (node->right) ? node->right->data.id : "(Kosong)");
    printf("  - Balance Factor: %d\n\n", getBalance(node));
    printAVLRecursive_Desc(node->left, node->data.id);
    printAVLRecursive_Desc(node->right, node->data.id);
}


// --- Bagian 3: Fungsi Wrapper Utama ---
void displayAVLAnalysis(AVLNode* root) {
    if (root == NULL) {
        printf("\n(Pohon Kosong)\n");
        return;
    }

    // Tampilkan Diagram
    printf("\n+--------------------- DIAGRAM POHON ----------------------+\n");
    printf("[%s] (BF: %d)\n", root->data.id, getBalance(root));
    if (root->right) printAVLRecursive_Graph(root->right, "", root->left == NULL ? 1 : 0);
    if (root->left) printAVLRecursive_Graph(root->left, "", 1);
    printf("+--------------------------------------------------------+\n");

    // Tampilkan Deskripsi Detail
    printf("\n+-------------------- DESKRIPSI DETAIL --------------------+\n");
    printAVLRecursive_Desc(root, NULL);
    printf("+--------------------------------------------------------+\n");
}