// tree.c (Visualisasi Diperbaiki dengan ASCII Standar)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"
#include "../include/sll.h" 

// --- FUNGSI INTI (Tidak berubah dari perbaikan sebelumnya) ---
// Bagian ini berisi createTreeNode, addChild, freeTree, findNodeInList, dan buildCitationTree
// yang sudah benar dari jawaban sebelumnya. Saya akan singkat di sini.

TreeNode* createTreeNode(Paper data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode) {
        newNode->data = data;
        newNode->child = NULL;
        newNode->sibling = NULL;
    }
    return newNode;
}

void addChild(TreeNode* parent, TreeNode* child) {
    if (!parent->child) {
        parent->child = child;
    } else {
        TreeNode* temp = parent->child;
        while (temp->sibling) {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->child);
    freeTree(root->sibling);
    free(root);
}

TreeNode* findNodeInList(TreeNode* head, const char* id) {
    TreeNode* current = head;
    while (current) {
        if (strcmp(current->data.id, id) == 0) {
            return current;
        }
        current = current->sibling;
    }
    return NULL;
}

TreeNode* buildCitationTree(Node* paperList) {
    if (!paperList) return NULL;
    TreeNode* allNodesHead = NULL, *allNodesTail = NULL;
    Node* sllCurrent = paperList;
    while (sllCurrent) {
        TreeNode* newNode = createTreeNode(sllCurrent->data);
        if (!allNodesHead) allNodesHead = allNodesTail = newNode;
        else { allNodesTail->sibling = newNode; allNodesTail = newNode; }
        sllCurrent = sllCurrent->next;
    }
    TreeNode* current = allNodesHead;
    while (current) {
        const char* citationId = current->data.incitation;
        if (citationId && strcmp(citationId, "-") != 0 && strcmp(citationId, "NULL") != 0) {
            TreeNode* parentNode = findNodeInList(allNodesHead, citationId);
            if (parentNode && parentNode != current) {
                addChild(parentNode, current);
            }
        }
        current = current->sibling;
    }
    TreeNode* root = NULL, *rootTail = NULL;
    current = allNodesHead;
    while (current) {
        TreeNode* nextNode = current->sibling;
        int hasParent = 0;
        TreeNode* finder = allNodesHead;
        while(finder) {
            TreeNode* childFinder = finder->child;
            while(childFinder) {
                if(childFinder == current) { hasParent = 1; break; }
                childFinder = childFinder->sibling;
            }
            if(hasParent) break;
            finder = finder->sibling;
        }
        if (!hasParent) {
            if (!root) root = rootTail = current;
            else { rootTail->sibling = current; rootTail = current; }
        }
        current = nextNode;
    }
    if (rootTail) rootTail->sibling = NULL;
    return root;
}


// ===================================================================
// ===         PERBAIKAN VISUALISASI ASCII STANDAR                 ===
// ===================================================================

/**
 * @brief Fungsi rekursif (helper) untuk mencetak node pohon dengan prefix yang benar.
 * @param node Node yang akan dicetak.
 * @param prefix String prefix yang menunjukkan indentasi dan garis cabang dari atas.
 * @param isLast Menandakan apakah node ini adalah anak terakhir dari induknya.
 */
void printTreeRecursive_StandardASCII(TreeNode* node, const char* prefix, int isLast) {
    if (!node) return;

    // 1. Cetak prefix yang sudah ada, diikuti dengan konektor cabang
    printf("%s", prefix);
    
    // Gunakan '\--' (backslash) untuk anak terakhir, dan '+--' untuk yang lain
    printf("%s", isLast ? "\\-- " : "+-- ");
    
    // 2. Cetak data node
    printf("[%s] %s (%d)\n", node->data.id, node->data.title, node->data.year);

    // 3. Siapkan prefix baru untuk anak-anak dari node ini
    char newPrefix[1024];
    
    // Jika node ini anak terakhir, prefix di bawahnya tidak perlu garis vertikal '|'
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "|   ");

    // 4. Rekursi untuk semua anak dari node ini
    TreeNode* child = node->child;
    while (child) {
        printTreeRecursive_StandardASCII(child, newPrefix, (child->sibling == NULL));
        child = child->sibling;
    }
}

/**
 * @brief Fungsi utama untuk visualisasi pohon menggunakan ASCII standar.
 * @param root Pointer ke node pertama di level root.
 * @param depth Parameter tidak digunakan, untuk kompatibilitas.
 */
void printTreeVisual(TreeNode* root, int depth) {
    if (!root) {
        printf("\n  [INFO] Tidak ada pohon untuk ditampilkan.\n");
        return;
    }

    printf("\n  +----------------------------------------------------+\n");
    printf("  |            VISUALISASI JEJARING SITASI             |\n");
    printf("  +----------------------------------------------------+\n");

    TreeNode* currentTreeRoot = root;
    while (currentTreeRoot) {
        // Cetak informasi root dengan jelas
        printf("\n> ROOT: %s (%d)\n", currentTreeRoot->data.title, currentTreeRoot->data.year);
        printf("  (ID: %s)\n", currentTreeRoot->data.id);
        
        // Mulai proses rekursif untuk anak-anak dari root ini
        TreeNode* child = currentTreeRoot->child;
        while (child) {
            printTreeRecursive_StandardASCII(child, "  ", (child->sibling == NULL));
            child = child->sibling;
        }
        
        currentTreeRoot = currentTreeRoot->sibling;
    }
     printf("\n  -- Akhir Visualisasi --\n");
}