// tree.c
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

TreeNode* findNodeById(TreeNode* root, const char* id) {
    if (!root) return NULL;
    if (strcmp(root->data.id, id) == 0) return root;

    TreeNode* found = findNodeById(root->child, id);
    if (found) return found;

    return findNodeById(root->sibling, id);
}

void insertCitationNode(TreeNode** root, Paper data) {
    TreeNode* newNode = createTreeNode(data);
    if (strcmp(data.incitation, "-") == 0 || strcmp(data.incitation, "NULL") == 0) {
        if (*root == NULL) {
            *root = newNode;
        } else {
            addChild(*root, newNode);  // Jika ada banyak root, anggap satu pohon besar
        }
    } else {
        TreeNode* parent = findNodeById(*root, data.incitation);
        if (parent) {
            addChild(parent, newNode);
        } else {
            // Jika parent belum ada (urutan belum sesuai), simpan sebagai root sementara
            addChild(*root, newNode);  // fallback
        }
    }
}

TreeNode* buildCitationTree(Node* paperList) {
    TreeNode* siti = NULL;
    Node* curr = paperList;
    while (curr) {
        insertCitationNode(&siti, curr->data);
        curr = curr->next;
    }
    return siti;
}

void printTreeVisual(TreeNode* root, int depth) {
    if (!root) return;

    for (int i = 0; i < depth; i++) {
        printf("  |   "); // Indentasi dengan garis vertikal ASCII
    }
    
    if (depth > 0) {
        printf("  +---"); // Cabang untuk child ASCII
    } else {
        printf("  *---"); // Root utama ASCII
    }
    
    printf(" [%s] %s (%d)\n", root->data.id, root->data.title, root->data.year);

    if (root->child) {
        printTreeVisual(root->child, depth + 1);
    }

    if (root->sibling) {
        printTreeVisual(root->sibling, depth); 
    }
}

void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->child);
    freeTree(root->sibling);
    free(root);
}
