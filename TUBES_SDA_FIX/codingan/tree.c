// src/tree.c

/*
 * File: tree.c
 * Deskripsi: Implementasi Pohon Sitasi (General N-ary Tree).
 * Bertujuan untuk memvisualisasikan hubungan hirarkis antar paper.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"

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

// Membangun pohon sitasi dengan algoritma Two-Pass untuk mengatasi 'orphan node'.
TreeNode* buildCitationTree(Node* paperList) {
    if (!paperList) return NULL;
    int count = 0;
    Node* counter = paperList;
    while (counter) {
        count++;
        counter = counter->next;
    }
    if (count == 0) return NULL;

    TreeNode** node_map = (TreeNode**)malloc(count * sizeof(TreeNode*));
    if (!node_map) return NULL;

    Node* current = paperList;
    for (int i = 0; i < count; i++) {
        node_map[i] = createTreeNode(current->data);
        current = current->next;
    }

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
            }
        }
    }
    free(node_map);
    return root;
}

// Fungsi rekursif untuk mencetak pohon sitasi dengan garis konektor.
void printTreeRecursive(TreeNode* node, char* prefix, int isLast) {
    if (!node) return;
    printf("%s", prefix);
    printf(isLast ? "`-- " : "|-- ");
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

// Fungsi utama untuk memulai visualisasi pohon sitasi.
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

// Membebaskan memori pohon sitasi.
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->child);
    freeTree(root->sibling);
    free(root);
}